#ifndef __SING_N_SPELL_INPUT_STATE
#define __SING_N_SPELL_INPUT_STATE

#include <mbed.h>

namespace io {
    /* The buttons */
    union inputstate_buttons_t {
        /* The value as a bitstring */
        uint8_t value;
        /* The value as a structured datatype */
        struct {
            /* The edit mode switch */
            uint8_t edit : 1;
            
            /* The prelisten key */
            uint8_t prelisten : 1;
            /* The save key */
            uint8_t save : 1;
            /* The save key */
            uint8_t load : 1;
            /* The next key */
            uint8_t save : 1;
            /* The prev key */
            uint8_t load : 1;
            
            /* The individual mode selector */
            uint8_t individual : 1; 
            /* The command/phoneme mode selector */
            uint8_t cmdphon : 1; 
        } buttons;
    };

    /* The input state */    
    struct inputstate_t {
        /* The alt key */
        uint8_t alt : 1;

        /* The buttons */
        inputstate_buttons_t buttons;

        /* The pitch encoder */
        int8_t pitch;
        /* The file selector */
        int8_t file;
        /* The value encoder */
        int8_t value;
        /* The selector */
        int8_t selector;
    };

    /* The output state */
    struct outputstate_t {
        /* The individual mode selector */
        uint8_t individual : 1; 
        /* The command/phoneme mode selector */
        uint8_t cmdphon : 1; 
    };
}

#endif