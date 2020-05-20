#include <mbed.h>
#include <rtos.h>

#include "code.h"

/* The table of legible names for a command */
const char* const tts_cmd_instruction[] = {
    "P0", "P10", "P20", "P70",
    "W3", "W6", "W9", 
    "FA", "SL", 
    "STS", "RLX", 
    "R", "D", "\x07P",
    "(", ")"
};
/* The table of legible names for a phoneme */
const char* const tts_phoneme_instruction[] = {
    "IY", "IH", "EY", "EH", "AY", 
    "AX", "UX", "OH", "AW", "OW", 
    "UH", "UW", "MM", "NE", "NO", 
    "NGE", "NGO", "LE", "LO", "WW", 
    "RR", "IYRR", "EYRR", "AXRR", "AWRR", 
    "OWRR", "EYIY", "OHIY", "OWIY", "OHIH", 
    "IYEH", "EHLL", "IYUW", "AXUW", "IHWW", 
    "AYWW", "OWWW", "JH", "VV", "ZZ", 
    "ZH", "DH", "BE", "BO", "EB", 
    "OB", "DE", "DO", "ED", "OD", 
    "GE", "GO", "EG", "OG", "CH", 
    "HE", "HO", "WH", "FF", "SE", 
    "SO", "SH", "TH", "TT", "TU", 
    "TS", "KE", "KO", "EK", "OK", 
    "PE", "PO", "R0", "R1", "R2", 
    "R3", "R4", "R5", "R6", "R7", 
    "R8", "R9", "A0", "A1", "A2", 
    "A3", "A4", "A5", "A6", "A7", 
    "A8", "A9", "B0", "B1", "B2", 
    "B3", "B4", "B5", "B6", "B7", 
    "B8", "B9", "C0", "C1", "C2", 
    "C3", "C4", "C5", "C6", "C7", 
    "C8", "C9", "D0", "D1", "D2", 
    "D3", "D4", "D5", "D6", "D7", 
    "D8", "D9", "D10", "D11", "M0", 
    "M1", "M2", "--"
 };

/* The table of legible names for a command */
const char* const tts_cmd_name[] = {
    "Pause 0ms", "Pause 100ms", "Pause 200ms", "Pause 700ms",
    "Wait 30ms", "Wait 60ms", "Wait 90ms", 
    "Fast", "Slow", 
    "Stress", "Relax", 
    "Repeat for", "Delay for", "Pitch",
    "Begin group", "End group"
};
/* The table of legible names for a phoneme */
const char tts_phoneme_long_vowel[] = "Long vowel";
const char tts_phoneme_short_vowel[] = "Short vowel";
const char tts_phoneme_nasal[] = "Nasal";
const char tts_phoneme_resonate[] = "Resonate";
const char tts_phoneme_color_vowel[] = "Color vowel";
const char tts_phoneme_diphtong[] = "Diphthong";
const char tts_phoneme_affricative[] = "Affricate";
const char tts_phoneme_fictive[] = "Fictive";
const char tts_phoneme_stop[] = "Voiced stop";
const char tts_phoneme_fricative[] = "Fricative";
const char tts_phoneme_robot[] = "Robot";
const char tts_phoneme_alarm[] = "Alarm";
const char tts_phoneme_beeps[] = "Beeps";
const char tts_phoneme_biological[] = "Biological";
const char tts_phoneme_dtmf[] = "DTMF";
const char tts_phoneme_misc[] = "Misc";
const char tts_phoneme_end[] = "End of word";
const char* const tts_phoneme_name[] = {
    tts_phoneme_long_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_long_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_long_vowel,
    tts_phoneme_short_vowel,
    tts_phoneme_long_vowel,
    tts_phoneme_nasal,
    tts_phoneme_nasal,
    tts_phoneme_nasal,
    tts_phoneme_nasal,
    tts_phoneme_nasal,
    tts_phoneme_resonate,
    tts_phoneme_resonate,
    tts_phoneme_resonate,
    tts_phoneme_resonate,
    tts_phoneme_color_vowel,
    tts_phoneme_color_vowel,
    tts_phoneme_color_vowel,
    tts_phoneme_color_vowel,
    tts_phoneme_color_vowel,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_diphtong,
    tts_phoneme_affricative,
    tts_phoneme_fictive,
    tts_phoneme_fictive,
    tts_phoneme_fictive,
    tts_phoneme_fictive,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_affricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_fricative,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_stop,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_robot,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_alarm,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_beeps,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_biological,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_dtmf,
    tts_phoneme_misc,
    tts_phoneme_misc,
    tts_phoneme_misc,
    tts_phoneme_end
 };

/* Check if value is a command or a phoneme */
synth::tts_code_type_t synth::tts_code_type(const uint8_t& code) {
    /* Check */
    if (code & 0x80) {
        /* Phonemes are values from 128 to 254 */
        return TTS_TYPE_PHONEME;
    } else if (code <= TTS_CMD_LAST) {
        /* A command must be castable to the above enum */
        return TTS_TYPE_COMMAND;
    } else {
        /* Invalid */
        return TTS_TYPE_INVALID;
    }
}

/* Go to next value in given range, for a given code */
void synth::tts_code_next(uint8_t& code) {
    /* Depending on its type, */
    switch (tts_code_type(code)) {
        /* If command */
        case TTS_TYPE_COMMAND: {
            /* Handle rollover if necessary */
            if (code == TTS_CMD_LAST) {
                code = TTS_CMD_FIRST;
            } else {
                ++code;
            }
            break;
        }

        /* If phoneme */
        case TTS_TYPE_PHONEME: {
            /* Increment and rollover */
            ++code;
            code = 0x80 | ((code) & 0x7F);
            break;
        }

        /* If invalid, bring back to phoneme type, and set to an end character */
        default: {
            code = TTS_PHON_END;
            break;
    /* Go to ith value in given range, for a given code */
    void tts_code_delta(uint8_t& code, int8_t delta);
        }
        
    }
}

/* Transform a code into its other type */
synth::tts_code_type_t synth::tts_code_transform(uint8_t& code) {
    /* Depending on its type, */
    switch (tts_code_type(code)) {
        /* If command */
        case TTS_TYPE_COMMAND:
            /* Just clamp to 0x7F range and set 0x80 bit */
            code = (code & 0x7F) | 0x80;
            // code = TTS_PHON_END;
            return TTS_TYPE_PHONEME;

        /* If data */
        case TTS_TYPE_PHONEME:
            /* Just modulo it to number of cmds and add first */
            code = ((code & 0x7F) % (TTS_CMD_LAST - TTS_CMD_FIRST + 1)) + TTS_CMD_FIRST; 
            return TTS_TYPE_COMMAND;

        /* If invalid */
        default:
            /* Set to end */
            code = TTS_PHON_END;
            return TTS_TYPE_PHONEME;
    }
}

/* Go to prev value in given range, for a given code */
void synth::tts_code_prev(uint8_t& code) {
    /* Depending on its type, */
    switch (tts_code_type(code)) {
        /* If command */
        case TTS_TYPE_COMMAND: {
            /* Handle rollover if necessary */
            if (code == TTS_CMD_FIRST) {
                code = TTS_CMD_LAST;
            } else {
                --code;
            }
            break;
        }

        /* If phoneme */
        case TTS_TYPE_PHONEME: {
            /* Increcrement and rollover */
            --code;
            code = 0x80 | (code & 0x7F);
            break;
        }

        /* If invalid, bring back to phoneme type, and set to an end character */
        default: {
            code = TTS_PHON_END;
            break;
        }
        
    }
}


/* Go to ith value in given range, for a given code */
void synth::tts_code_delta(uint8_t& code, int8_t delta) {
    /* Depending on its type, */
    switch (tts_code_type(code)) {
        /* If command */
        case TTS_TYPE_COMMAND: {
            code += (uint8_t)delta;
            code = (code % (TTS_CMD_LAST - TTS_CMD_FIRST + 1)) + TTS_CMD_FIRST;
            break;
        }

        /* If phoneme */
        case TTS_TYPE_PHONEME: {
            /* Increment and rollover */
            code += (uint8_t)delta;
            code = 0x80 | ((code) & 0x7F);
            break;
        }

        /* If invalid, don't touch anything */
        default: { break; }
    }
}

/* Return the name of a code */
const char* synth::tts_code_instruction(const uint8_t& code) {
    /* Depending on its type, */
    switch (tts_code_type(code)) {
        /* If command */
        case TTS_TYPE_COMMAND:
            return tts_cmd_instruction[code];

        /* If phoneme */
        case TTS_TYPE_PHONEME:
            return tts_phoneme_instruction[code & 0x7F];

        /* If invalid, bring back to phoneme type, and set to an end character */
        default:
            return nullptr;            
    }
}

/* Return the name of a code */
const char* synth::tts_code_name(const uint8_t& code) {
    /* Depending on its type, */
    switch (tts_code_type(code)) {
        /* If command */
        case TTS_TYPE_COMMAND:
            return tts_cmd_name[code];

        /* If phoneme */
        case TTS_TYPE_PHONEME:
            return tts_phoneme_name[code & 0x7F];

        /* If invalid, bring back to phoneme type, and set to an end character */
        default:
            return nullptr;        
    }
}