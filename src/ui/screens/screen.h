#ifndef __SING_N_SPELL_UI_SCREEN
#define __SING_N_SPELL_UI_SCREEN

#include <mbed.h>

#include "SerialLCD.h"

namespace ui {

    /* Function that renders a screen */
    typedef void (*render_fun_t)(void*, SerialLCD*);
    /* Function that updates a screen */
    typedef void (*update_fun_t)(void*, bool*);

    /* Represents a screen */
    struct screen_t {
        /* The state */
        void* state;
        /* The render method */
        render_fun_t render;
        /* The update method */
        update_fun_t update;
    };

};

#endif