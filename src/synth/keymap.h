#ifndef __SING_N_SPELL_SYNTH_KEYMAP
#define __SING_N_SPELL_SYNTH_KEYMAP

#include <mbed.h>
#include <rtos.h>

#include "synth/tts/lexicon.h"

#define KEYMAP_NOTE_START       24
#define KEYMAP_NOTE_LENGTH      12*5
#define KEYMAP_NOTE_END         KEYMAP_NOTE_START + KEYMAP_NOTE_LENGTH

namespace synth {

    struct key_t {
        /* The corresponding note */
        uint8_t note;
        /* The pitch */
        uint8_t pitch;
        /* The corresponding voice sample */
        word_t* word;
    };

    struct keymap_t {
        /* The global word */
        word_t* global;
        /* The list of special override from keys */
        key_t* keys[KEYMAP_NOTE_LENGTH];
    };

};

#endif