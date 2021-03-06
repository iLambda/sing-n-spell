#include "audio/dev/speakjet.h"
#include "synth/tts/code.h"
#include "utils/math.h"
#include "utils/debugging.h"
#include "tts2speakjet.h"

/* Create a translator */
audio::codec::TTS2Speakjet::TTS2Speakjet(const synth::worditerator_t& it, float frequency, uint8_t speed, bool sustain) : m_source(it) {
    /* Initialize */
    this->m_initialPosition = this->m_source.position();
    this->m_sustainEnable = false;
    utils::preserved_constant<float>(this->m_frequency, frequency);
    utils::preserved_constant<uint8_t>(this->m_speed, speed);
    /* Reset the state */
    this->reset();
}

/* Reset the current translator state */
void audio::codec::TTS2Speakjet::reset() {
    /* Reset iterator to initial position */
    this->m_source.select(this->m_initialPosition);
    /* Reset the data mode */
    this->m_longCmd = true;
    /* Say we're in initial state */
    this->m_state = STATE_INITIAL;
    /* Reset sustain */
    this->m_sustainStartIndex = 0;
    this->m_sustainEndIndex = 0;
    this->m_isSustaining = false;
}

/* Apply transition */
MBED_FORCEINLINE uint8_t HzToByte(float freq) {
    return (uint8_t)__USAT(utils::uround(freq), 8);
}

inline bool advance(synth::worditerator_t& it) {
    /* Peek next */
    uint8_t peeked = 0xFF;
    /* If not ok, can't advance */
    if (!it.peek(peeked)) { return false; }
    /* There is a next element. If end, can't advance */
    if (peeked == 0xFF) { return false; }
    /* Advance */
    return it.next();
}

inline bool canAdvance(const synth::worditerator_t& it) {
    /* Peek next */
    uint8_t peeked = 0xFF;
    /* If not ok, can't advance */
    if (!it.peek(peeked)) { return false; }
    /* There is a next element. If end, can't advance */
    if (peeked == 0xFF) { return false; }
    /* Can advance ! */
    return true;
}

/* Get next produced code. Returns iff a code was indeed output */
bool audio::codec::TTS2Speakjet::next(uint8_t& out, bool* instantaneous) {

    /* Macro */
    #define __RESET() { this->m_longCmd = true; }

    #define PRODUCE(v) { if (instantaneous) { *instantaneous = (v); } }

    #define NOP() { break; }
    #define OUT(c, i) { out = (c); PRODUCE(i); break; }
    #define OUT_LONG(c, d, i) OUT(m_longCmd ? (c) : (d), i)

    #define GOTO(s) { __RESET(); this->m_state = (s); break; }
    #define GOTO_LONG(s) { if (this->m_longCmd)  { this->m_longCmd = false; } else { __RESET(); this->m_state = (s); } break; }
    #define GOTO_EPSI(s) { __RESET(); this->m_state = (s); spent = !next(out); break; }
    #define GOTO_NEXT(s) { bool n = advance(this->m_source); GOTO(n ? (s) : STATE_SPENT); }
    #define GOTO_EPSI_NEXT(s) { bool n = advance(this->m_source); GOTO_EPSI(n ? (s) : STATE_SPENT); }


    /* Check if iterator is null */
    if (this->m_source.word() == nullptr) {
        /* We're already spent */
        this->m_state = STATE_SPENT;
    }

    /* Were we spent when entering ? */
    bool spent = this->m_state == STATE_SPENT;   

    /* Produce an output for this state */
    switch (this->m_state) {
        /* Initial, final & ready states states */
        case STATE_INITIAL:             NOP()
        case STATE_SPENT:               NOP()
        case STATE_READY:               NOP()
        /* Send pitch & speed */
        case STATE_PITCH:               OUT_LONG(SPEAKJET_CODE_PITCH, HzToByte(this->m_frequency.current), true)
        case STATE_SPEED:               OUT_LONG(SPEAKJET_CODE_SPEED, this->m_speed.current, true)
        /* Send a byte */
        case STATE_DATA_SKIP:           NOP()
        case STATE_DATA_PHONEME:        OUT(this->m_source.get(), false)
        case STATE_DATA_CMD_TRANS:      OUT(this->m_source.get(), true)
    }    

    /* Do transition */
    switch (this->m_state) {
        /* Initial & final state */
        case STATE_INITIAL:             GOTO_EPSI(STATE_PITCH)
        case STATE_SPENT:               NOP()
        /* Pitch & speed */
        case STATE_PITCH:               GOTO_LONG(STATE_READY)
        case STATE_SPEED:               GOTO_LONG(STATE_READY)
        /* Byte sending states */
        case STATE_DATA_SKIP:           GOTO_EPSI_NEXT(STATE_READY)
        case STATE_DATA_PHONEME:        GOTO_NEXT(STATE_READY)
        case STATE_DATA_CMD_TRANS:      GOTO_NEXT(STATE_READY)
        /* Ready */
        case STATE_READY: {
            /* If sustain is disabled, and we're in the sustain zone, cut short */
            if (!this->m_sustainEnable && this->m_isSustaining) {
                /* Sustain is now done */
                this->m_isSustaining = false;
                /* If no end index, just go to the end of the stream */
                auto endIdx = 
                    this->m_sustainEndIndex 
                        ? this->m_sustainEndIndex
                        : this->m_source.length();
                /* Go to it */
                this->m_source.select(endIdx);
            }
            /* Get type of current data */ 
            auto code = this->m_source.get();
            auto type = synth::tts_code_type(code);
            /* Check it */
            switch (type) {
                case synth::TTS_TYPE_PHONEME: GOTO_EPSI(STATE_DATA_PHONEME)
                case synth::TTS_TYPE_INVALID: GOTO_EPSI(STATE_DATA_SKIP)
                case synth::TTS_TYPE_COMMAND: {
                    /* Check if command is transparent */
                    if (synth::tts_code_transparent(code)) {
                        /* Go to command passthrough mode */
                        GOTO_EPSI(STATE_DATA_CMD_TRANS)
                    } else {
                        /* Check which command it actually is */
                        switch (code) {
                            /* Sustain */
                            case synth::TTS_CMD_SUSTAIN_OPEN: {
                                /* Save sustain index as next element */
                                this->m_sustainStartIndex = this->m_source.position() + 1;
                                /* If sustain is enabled, we're in the sustain zone */
                                if (this->m_sustainEnable) {
                                    this->m_isSustaining = true;
                                }
                                /* We're done ; skip current code since it is purely imaginary */
                                GOTO_EPSI(STATE_DATA_SKIP);
                            }
                            case synth::TTS_CMD_SUSTAIN_END: {
                                /* Set sustain end index */
                                this->m_sustainEndIndex = this->m_source.position() + 1;
                                /* Check if sustain enabled, and index is valid (!= 0, and section nonempty) */
                                if (this->m_sustainEnable && 
                                    this->m_sustainStartIndex &&
                                    this->m_source.position() > this->m_sustainStartIndex) {
                                    /* Rewind position of the iterator */
                                    this->m_source.select(this->m_sustainStartIndex - 1);
                                } else {
                                    /* We're not sustaining anymore */
                                    this->m_isSustaining = false;
                                }
                                /* We're done ; skip current code since it is purely imaginary */
                                GOTO_EPSI(STATE_DATA_SKIP);
                            }
                            
                            /* Skip (TODO: error)*/
                            default: GOTO_EPSI(STATE_DATA_SKIP);
                        }
                    }
                    break;
                }            
            }
            break;
        }
    }    

    /* Return true iff we produced something, that is, if we're not spent */
    return !spent;
} 