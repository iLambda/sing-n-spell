#ifndef __SING_N_SPELL_UI_SCREEN
#define __SING_N_SPELL_UI_SCREEN

#include <mbed.h>
#include <serlcd.h>

#include "io/state.h"

namespace ui {

    /* Function called on entering a screen */
    typedef void (*reset_fun_t)(void*);
    /* Function that renders a screen */
    typedef void (*render_fun_t)(void*, SerialLCD*);
    /* Function that updates a screen */
    typedef void (*update_fun_t)(void*, bool*);
    /* Function that handles input in a screen */
    typedef void (*input_fun_t)(void*, const io::inputstate_t& input, io::outputstate_t& output);

    /* Represents a screen */
    struct screen_t {
        /* The state */
        void* state;
        /* The enter method */
        reset_fun_t reset;
        /* The render method */
        render_fun_t render;
        /* The update method */
        update_fun_t update;
        /* The input method */
        input_fun_t input;
    };

};

#endif