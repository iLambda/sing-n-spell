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
            /* The prelisten key */
            uint8_t prelisten : 1;
            /* The save key */
            uint8_t save : 1;
            /* The save key */
            uint8_t load : 1;
            /* The next key */
            uint8_t next : 1;
            /* The prev key */
            uint8_t prev : 1;
            /* The selector btn */
            uint8_t selector : 1;
            
            /* The individual mode selector */
            uint8_t individual : 1; 
            /* The command/phoneme mode selector */
            uint8_t cmdphon : 1; 
        };
    };

    #define CTRL_IN_BUS_BTN(prelisten, save, load, next, prev, selector, indiv, cmdphon) \
        (prelisten), (save), (load), (next), (prev), (selector), (indiv), (cmdphon)
    

    /* The encoders */
    union inputstate_encoders_t {
        /* The value as a structured datatype */
        MBED_PACKED(struct) {
            /* The pitch encoder */
            int8_t pitch;
            /* The file selector */
            int8_t file;
            /* The data encoder */
            int8_t data;
            /* The selector */
            int8_t selector;
        };
        /* The value as a bitstring */
        uint32_t value;
    };

    #define CTRL_IN_BUS_ENC(pitch_pos, pitch_neg, file_pos, file_neg, data_pos, data_neg, selector_pos, selector_neg) \
        (pitch_pos), (pitch_neg), (file_pos), (file_neg), (data_pos), (data_neg), (selector_pos), (selector_neg)
    
    

    /* The input state */    
    struct inputstate_t {
        /* The buttons */
        inputstate_buttons_t buttons;
        /* The encoders */
        inputstate_encoders_t encoders;
        /* The alt key */
        uint8_t alt : 1;
        /* The edit mode switch */
        uint8_t edit : 1;
    };

    /* The output state */
    union outputstate_t {
        struct {
            /* The individual mode selector */
            uint8_t individual : 1; 
            /* The command/phoneme mode selector */
            uint8_t cmdphon : 1; 
        };
        /* The value */
        uint8_t value;
    };
    
    #define CTRL_OUT_BUS(individual, cmdphon) (individual), (cmdphon)
}

#endif