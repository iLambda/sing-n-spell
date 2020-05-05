#ifndef __SING_N_SPELL_INPUT_MIDI
#define __SING_N_SPELL_INPUT_MIDI

#define MIDI_BAUD_RATE      31250

#include <mbed.h>
#include <rtos.h>

namespace io {

    const char* const notes[12] = 
        { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };

    enum midimsgtype_t {
        MIDI_TYPE_CC = 0xB0, 
        MIDI_TYPE_NOTEON = 0x90, 
        MIDI_TYPE_NOTEOFF = 0x80, 
        MIDI_TYPE_AFTERTOUCH = 0xA0, 
        MIDI_TYPE_PITCHBEND = 0xE0, 
        MIDI_TYPE_PROGCHANGE = 0xC0, 
        MIDI_TYPE_CHANPRESSURE = 0xD0, 
        MIDI_TYPE_CLOCK = 0xF8, 
        MIDI_TYPE_TICK = 0xF9, 
        MIDI_TYPE_START = 0xFA, 
        MIDI_TYPE_CONTINUE = 0xFB, 
        MIDI_TYPE_STOP = 0xFC, 
        MIDI_TYPE_ACTIVESENSE = 0xFE, 
        MIDI_TYPE_RESET = 0xFF, 
        MIDI_TYPE_TC_QUARTERFRAME = 0xF1, 
        MIDI_TYPE_SONGPOSITION = 0xF2, 
        MIDI_TYPE_SONGSELECT = 0xF3, 
        MIDI_TYPE_TUNEREQUEST = 0xF6
    };

    enum midicc_t {
        MIDI_CC_ALLSOUNDOFF = 120,
        MIDI_CC_RESET = 121,
        MIDI_CC_NOTEOFF_ALL = 123,
        MIDI_CC_NOTEOFF_NOOMNI = 124,
        MIDI_CC_NOTEOFF_OMNI = 125,
        MIDI_CC_NOTEOFF_MONO = 126,
        MIDI_CC_NOTEOFF_POLY = 127,
    };

    struct midimsg_t {
        /* The status byte */
        union {
            /* The value of the status byte */
            uint8_t value;
            /* The command and the channel */
            struct {
                uint8_t command : 4;
                uint8_t channel : 4;
            };
            /* The command and the channel */
            struct {
                uint8_t realtime : 4;
                uint8_t order : 4;
            };

        } status;
        /* The data */
        union {
            /* Value of the data as a 16-bit integer */
            uint16_t value;
            /* The two data bytes */
            struct {
                uint8_t byte1;
                uint8_t byte2;
            };
            struct {
                uint8_t key;
                uint8_t velocity;
            };
            struct {
                uint8_t controller;
                uint8_t change;
            };
        } data;
    };

    /* Return midi message length */
    inline int8_t midi_message_len(uint8_t status) {
        /* Check */
        switch(status & 0xF0){
            /* 3 byte long messages */
            case MIDI_TYPE_CC:
            case MIDI_TYPE_NOTEON:
            case MIDI_TYPE_NOTEOFF:
            case MIDI_TYPE_AFTERTOUCH:
            case MIDI_TYPE_PITCHBEND:
                return 3;

            /* 2 byte long messages */
            case MIDI_TYPE_PROGCHANGE:
            case MIDI_TYPE_CHANPRESSURE:
            case MIDI_TYPE_SONGSELECT:
                return 2;

            /* Handle the midi realtime case */
            case 0xF0:
                switch(status) {
                    /* One byte midi messages */
                    case MIDI_TYPE_CLOCK:
                    case MIDI_TYPE_TICK:
                    case MIDI_TYPE_START:
                    case MIDI_TYPE_CONTINUE:
                    case MIDI_TYPE_STOP:
                    case MIDI_TYPE_ACTIVESENSE:
                    case MIDI_TYPE_RESET:
                    case MIDI_TYPE_TUNEREQUEST:
                        return 1;
                    /* Three byte midi message */
                    case MIDI_TYPE_SONGPOSITION:
                        return 3;
                    /* Two bytes midi message */
                    case MIDI_TYPE_TC_QUARTERFRAME:
                    case MIDI_TYPE_SONGSELECT:
                        return 2;
                    /* Undefined case */
                    default:
                        return -1;
                }

            /* Undefined case */
            default:
                return -1;
        }
    }
    /* Return midi message length */
    inline int8_t midi_message_len(const midimsg_t& msg) {
        return midi_message_len(msg.status.value);
    }
    /* Return midi message length */
    inline midimsgtype_t midi_message_type(const midimsg_t& msg) {
        return (msg.status.value & 0xF0) != 0xF0 
                /* Usual command */
                ? (midimsgtype_t)(msg.status.value & 0xF0)
                /* Realtime command */
                : (midimsgtype_t)(msg.status.value);
    }
    /* Is this a special CC msg ? */
    inline bool midi_is_special_cc(uint8_t controller) {
        return controller < MIDI_CC_ALLSOUNDOFF;
    }
    /* Note to string */
    inline void midi_note_to_cstr(char* str, uint8_t note) {
        /* Store intermediate result */
        uint8_t tmp = __UQSUB8(note & 0x7F, 21);
        /* Compute octave and note */
        uint8_t semitone = tmp % 12;
        uint8_t octave = (tmp + 9) / 12;
        /* Put note together */
        strcpy(str, notes[semitone]);
        itoa(octave, str + strlen(str), 10);
    }
};

#endif