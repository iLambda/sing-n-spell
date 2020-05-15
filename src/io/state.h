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
            
            /* The individual mode selector */
            uint8_t individual : 1; 
            /* The command/phoneme mode selector */
            uint8_t cmdphon : 1; 
        };
    };

    /* The encoders */
    union inputstate_encoders_t {
        /* The value as a bitstring */
        uint32_t value;
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
    };
    

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

    /* Accumulate an input state */
    inline void accumulate(inputstate_t& accumulator, const inputstate_t& delta) {
        /* Accumulate alt */
        accumulator.alt |= delta.alt;
        /* Accumulate buttons */
        accumulator.buttons.value |= delta.buttons.value;
        /* Accumulate encoders */        
        accumulator.encoders.value = 
            __QADD8(accumulator.encoders.value, delta.encoders.value);
        /* Edit is not accumulated */
        accumulator.edit = delta.edit;
    }

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

    #define CTRL_BUS_OUT(indiv, cmdphon) (indiv),(cmdphon)
}

#endif