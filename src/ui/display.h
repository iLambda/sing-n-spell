#ifndef __SING_N_SPELL_UI_DISPLAY
#define __SING_N_SPELL_UI_DISPLAY

#define UI_DISPLAY_MAX_SCREENS      16
#define UI_DISPLAY_REFRESH_UI_RATE  10

#define UI_DISPLAY_THREAD_PRIORITY_EVENT      osPriorityNormal
#define UI_DISPLAY_THREAD_PRIORITY_UI         osPriorityNormal
#define UI_DISPLAY_THREAD_QUEUE_SIZE          16
#define UI_DISPLAY_THREAD_FLAG_UI_DIRTY       0x01
#define UI_DISPLAY_THREAD_FLAG_UI_CLEAR       0x02
#define UI_DISPLAY_THREAD_FLAG_EVENT_DRAWN    0x01

#define UI_DISPLAY_SERLCD_TX                  NC

#include <mbed.h>
#include <rtos.h>
#include <serlcd.h>

#include "io/state.h"
#include "ui/screen.h"

namespace ui {

    class Display {

        public:
            struct screenevent_t {
                /* A pointer to the screen */
                screen_t* screen;
                /* Its ID */
                uint8_t id;
            };

        private:
            /* The UI thread */
            static Thread m_threadUi;
            /* The event thread */
            static Thread m_threadEvent;
            /* The screen queue */
            static Queue<screenevent_t, UI_DISPLAY_THREAD_QUEUE_SIZE> m_screenQueue;
            /* The message pool */
            static MemoryPool<screenevent_t, UI_DISPLAY_THREAD_QUEUE_SIZE> m_screenEventPool;
            
            /* The display */
            static SerialLCD* m_display;
            /* The registered screens */
            static screen_t m_screens[UI_DISPLAY_MAX_SCREENS];
            /* The current screen id */
            static int8_t m_currentScreen;

            /* The input screen mutex */
            static Mutex m_inputMutex;
            

        private:
            /* Create a display */
            Display() {}

        public:
            /* Run the display */
            static void run();

            /* Add a new screen */
            static bool add(uint8_t id, const screen_t& screen);
            /* Add a new screen */
            template <typename T> __STATIC_FORCEINLINE bool add(T& screen) {
                return Display::add(screen.getID(), screen.get());
            }
            /* Get the screen */
            static bool get(uint8_t id, screen_t& screen);
            /* Go to screen */
            static bool go(uint8_t id);
            /* Go to screen */
            template <typename T> __STATIC_FORCEINLINE bool go(T& screen) {
                return Display::go(screen.getID());
            }
            /* The current screen id */
            static int8_t current();
            /* Wait until screen repaint is over (WORKS ONLY IN EVENT THREAD !) */
            static void dry();

            /* The screen width */
            __STATIC_FORCEINLINE constexpr uint8_t screenWidth() { return 20; }
            /* The screen height */
            __STATIC_FORCEINLINE constexpr uint8_t screenHeight() { return 4; }

        private:
            /* Repaint */
            static void repaint(const bool& clear);
            /* Dirty */
            static void dirty(const bool& clear);
            /* Draw the frame */
            static void drawFrame();

            /* The input handler */
            static void inputHandler(const io::inputstate_t& inputs);
            /* The UI thread */
            static void uiThread();
            /* The event thread */
            static void eventThread();

    };

};

#endif