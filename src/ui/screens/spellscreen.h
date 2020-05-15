#ifndef __SING_N_SPELL_UI_SCREEN_SPELLSCREEN
#define __SING_N_SPELL_UI_SCREEN_SPELLSCREEN

#include <mbed.h>
#include <serlcd.h>

#include "ui/screen.h"

namespace ui {
    namespace screen {

        class SpellScreen {   

            private:
                /* The cursor position */
                struct { 
                    uint8_t x : 4;
                    uint8_t y : 4;
                } m_cursor;
                /* The dirty flags */
                union dirtyflags_t {
                    /* The value as an integer */
                    uint32_t value;
                    /* The flags */
                    struct {
                        uint8_t keyboard : 1;
                        uint8_t cursor : 1;
                    };
                } m_isDirty;

            public:
                SpellScreen() {} 

            public:
                /* Return a screen_t representing this screen */
                screen_t get() { return { (void*)this, &reset, &render, &update, &input }; }
                /* Return the ID of this screen */
                static constexpr uint8_t getID() { return 2; }

            private:
                /* Parse inputs */
                static void input(void* state, const io::inputstate_t& inputs, io::outputstate_t& outputs);
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