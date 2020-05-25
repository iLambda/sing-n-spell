#ifndef __SING_N_SPELL_INPUT_MIDI
#define __SING_N_SPELL_INPUT_MIDI

#define MIDI_BAUD_RATE      31250

#include <mbed.h>
#include <rtos.h>

#include <libmidi.h>

namespace io {

    const char* const notes[12] = 
        { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };

    enum midimsgtype_t : uint8_t{
        MIDI_TYPE_NOTEOFF = 0x80, 
        MIDI_TYPE_NOTEON = 0x90, 
        MIDI_TYPE_AFTERTOUCH = 0xA0, 
        MIDI_TYPE_CC = 0xB0, 
        MIDI_TYPE_PROGCHANGE = 0xC0, 
        MIDI_TYPE_CHANPRESSURE = 0xD0, 
        MIDI_TYPE_PITCHBEND = 0xE0, 
        MIDI_TYPE_TC_QUARTERFRAME = 0xF1, 
        MIDI_TYPE_SONGPOSITION = 0xF2, 
        MIDI_TYPE_SONGSELECT = 0xF3, 
        MIDI_TYPE_TUNEREQUEST = 0xF6,
        MIDI_TYPE_CLOCK = 0xF8, 
        MIDI_TYPE_TICK = 0xF9, 
        MIDI_TYPE_START = 0xFA, 
        MIDI_TYPE_CONTINUE = 0xFB, 
        MIDI_TYPE_STOP = 0xFC, 
        MIDI_TYPE_ACTIVESENSE = 0xFE, 
        MIDI_TYPE_RESET = 0xFF
    };

    enum midicc_t : uint8_t{
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
        uint8_t status;
        /* The data */
        union {
            /* The two data bytes */
            MBED_PACKED(struct) {
                uint8_t data1;
                uint8_t data2;
            };
            MBED_PACKED(struct) {
                uint8_t key;
                uint8_t velocity;
            };
            MBED_PACKED(struct) {
                uint8_t controller;
                uint8_t change;
            };
        };
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
        return midi_message_len(msg.status);
    }
    /* Return midi message type */
    MBED_FORCEINLINE midimsgtype_t midi_message_type(const midimsg_t& msg) {
        return (msg.status & 0xF0) != 0xF0 
                /* Usual command */
                ? (midimsgtype_t)(msg.status & 0xF0)
                /* Realtime command */
                : (midimsgtype_t)(msg.status);
    }
    /* Return midi message channel */
    MBED_FORCEINLINE uint8_t midi_message_channel(const midimsg_t& msg) {
        return (msg.status & 0xF0) != 0xF0 ? (msg.status & 0x0F) : 0xFF;
    }

    MBED_FORCEINLINE int16_t midi_message_bend(const midimsg_t& msg) {
        return (int16_t)((((uint16_t)msg.data1 & 0x007F) | ((uint16_t)msg.data2 << 7)) & 0x3FFF) - 0x2000;
    }

    /* From libmidi */
    MBED_FORCEINLINE void midi_of_libmidi(midimsg_t& msg, libmidi_event_type evt, uint8_t chan, uint8_t data1, uint8_t data2) {
        /* Fill */
        switch (evt) {
            /* Realtime events */
            case EVT_SYS_REALTIME_TIMING_CLOCK: {
                msg.status = MIDI_TYPE_CLOCK;
                msg.data1 = 0x00;
                msg.data2 = 0x00;
                break;
            }
            case EVT_SYS_REALTIME_SEQ_START: {
                msg.status = MIDI_TYPE_START;
                msg.data1 = 0x00;
                msg.data2 = 0x00;
                break;
            } 
            case EVT_SYS_REALTIME_SEQ_CONTINUE: {
                msg.status = MIDI_TYPE_CONTINUE;
                msg.data1 = 0x00;
                msg.data2 = 0x00;
                break;
            } 
            case EVT_SYS_REALTIME_SEQ_STOP: {
                msg.status = MIDI_TYPE_STOP;
                msg.data1 = 0x00;
                msg.data2 = 0x00;
                break;
            }
            case EVT_SYS_REALTIME_ACTIVE_SENSE: {
                msg.status = MIDI_TYPE_ACTIVESENSE;
                msg.data1 = 0x00;
                msg.data2 = 0x00;
                break;
            } 
            case EVT_SYS_REALTIME_RESET: {
                msg.status = MIDI_TYPE_RESET;
                msg.data1 = 0x00;
                msg.data2 = 0x00;
                break;
            } 
            
            /*
            * Channel messages.
            */
            case EVT_CHAN_NOTE_OFF: {
                msg.status = MIDI_TYPE_NOTEOFF | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = data2;
                break;
            } 
            case EVT_CHAN_NOTE_ON: {
                /* If velocity is 0, parse as note off */
                msg.status = (data2 ? MIDI_TYPE_NOTEON : MIDI_TYPE_NOTEOFF) | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = data2;
                break;
            } 
            case EVT_CHAN_POLY_AFTERTOUCH: {
                msg.status = MIDI_TYPE_AFTERTOUCH | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = data2;
                break;
            } 
            case EVT_CHAN_CONTROL_CHANGE: {
                msg.status = MIDI_TYPE_CC | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = data2;
                break;
            } 
            case EVT_CHAN_PROGRAM_CHANGE: {
                msg.status = MIDI_TYPE_PROGCHANGE | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = 0x00;
                break;
            } 
            case EVT_CHAN_AFTERTOUCH: {
                msg.status = MIDI_TYPE_CHANPRESSURE | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = 0x00;
                break;
            } 
            case EVT_CHAN_PITCH_BEND: {
                msg.status = MIDI_TYPE_PITCHBEND | (chan & 0x0F);
                msg.data1 = data1;
                msg.data2 = data2;
                break;
            } 

            /* Unhandled ; ignore */
            default: return;
        }
    }

    /* Is this a special CC msg ? */
    MBED_FORCEINLINE bool midi_is_special_cc(uint8_t controller) {
        return controller >= MIDI_CC_ALLSOUNDOFF;
    }

    /* Note to freq */
    inline float midi_note_to_freq(uint8_t note) {
        int a = 440;
        return (a / 32.0f) * pow(2.0f, ((((float)note) - 9.0f) / 12.0f));
    }

    /* Note to string */
    inline uint8_t midi_note_to_cstr(char* str, uint8_t note) {
        /* Store intermediate result */
        uint8_t tmp = __UQSUB8(note & 0x7F, 21);
        /* Compute octave and note */
        uint8_t semitone = tmp % 12;
        uint8_t octave = (tmp + 9) / 12;
        /* Put note together */
        strcpy(str, notes[semitone]);
        uint8_t notelen = strlen(str);
        itoa(octave, str + strlen(str), 10);
        /* Return length */
        return notelen + 1;
    }
};

#endif