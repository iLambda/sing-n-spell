#ifndef __SING_N_SPELL_UI_SCREEN_BOOTSCREEN
#define __SING_N_SPELL_UI_SCREEN_BOOTSCREEN

#include <mbed.h>
#include <serlcd.h>

#include "ui/screen.h"

#define BOOTSCREEN_DURATION         5000

namespace ui {
    namespace screen {

        class BootScreen {

            private:
                uint32_t m_frame;

            public:
                BootScreen() {}            

            public:
                /* Return a screen_t representing this screen */
                screen_t get() { return { (void*)this, &reset, &render, &update, nullptr }; }
                /* Return the ID of this screen */
                static constexpr uint8_t getID() { return 0; }

            private:
                /* Enter the screen */
                static void reset(void* state);
                /* Render the screen */
                static void render(void* state, SerialLCD* display);
                /* Update the screen */
                static void update(void* state, bool* dirty);

        };
    }

};

#endif