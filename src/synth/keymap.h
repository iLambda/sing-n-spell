#ifndef __SING_N_SPELL_SYNTH_KEYMAP
#define __SING_N_SPELL_SYNTH_KEYMAP

#include <mbed.h>
#include <rtos.h>

#include "synth/tts/lexicon.h"

#define KEYMAP_LENGTH           12*5
#define KEYMAP_NOTE_FIRST       12
#define KEYMAP_NOTE_LAST        KEYMAP_NOTE_FIRST + KEYMAP_LENGTH - 1

namespace synth {

    enum keymode_t {
        KEY_MODE_GLOBAL = 0,
        KEY_MODE_LOCAL = 1
    };

    struct keyentry_t {
        /* The pitch */
        uint8_t pitch;
        /* The key ode */
        keymode_t mode;
        /* The corresponding voice sample */
        word_t* word;
        /* The last selected index */
        uint8_t lastIdx;
    };

    struct keymap_t {
        /* The global word */
        word_t* global;
        /* The last selected index for the global word */
        uint8_t lastGlobalIdx;
        /* The list of special override from keys */
        keyentry_t keys[KEYMAP_LENGTH];
    };

};

#endif