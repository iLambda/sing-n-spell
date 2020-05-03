#ifndef __SING_N_SPELL_UI_SCREEN_BOOTSCREEN
#define __SING_N_SPELL_UI_SCREEN_BOOTSCREEN

#include <mbed.h>
#include <serlcd.h>

#include "screen.h"

namespace ui {
    namespace screen {

        class BootScreen {

            private:
                int x = 0;

            public:
                static const uint8_t SCREEN_ID;

            public:
                BootScreen();

            public:
                

            public:
                /* Return a screen_t representing this screen */
                screen_t get() { return { (void*)this, &render, &update }; }
                /* Return the ID of this screen */
                static uint8_t getID() { return SCREEN_ID; }

            private:
                /* Render the screen */
                static void render(void* state, SerialLCD* display);
                /* Update the screen */
                static void update(void* state, bool* dirty);

        };
    }

};

#endif