#include "audio/dev/speakjet.h"
#include "utils/math.h"
#include "tts2speakjet.h"
#include "utils/debugging.h"

/* Create a translator */
audio::codec::TTS2Speakjet::TTS2Speakjet(const synth::worditerator_t& it, float frequency) : m_source(it) {
    /* Initialize */
    this->m_initialPosition = this->m_source.position();
    utils::preserved_constant<float>(this->m_frequency, frequency);
    utils::preserved_constant<uint8_t>(this->m_speed, TTS2SPEAKJET_SPEED_DEFAULT);
    /* Reset the state */
    this->reset();
}

/* Reset the current translator state */
void audio::codec::TTS2Speakjet::reset() {
    /* Reset iterator to initial position */
    this->m_source.select(this->m_initialPosition);
    /* Say we're in initial state */
    this->m_state = STATE_INITIAL;
}

/* Apply transition */
MBED_FORCEINLINE uint8_t HzToByte(float freq) {
    return (uint8_t)__USAT(utils::uround(freq), 8);
}


/* Get next produced code. Returns amount of codes left to be produced by current word command. */
bool audio::codec::TTS2Speakjet::next(uint8_t& out) {

    /* Macro */
    #define OUT(c) { out = (c); break; }
    #define GOTO(s) { this->m_state = (s); break; }

    /* Were we spent when entering ? */
    bool spent = this->m_state == STATE_SPENT;

    /* Check if iterator is null */
    if (this->m_source.word() == nullptr) {
        return false;
    }

    /* Handle initial state (epsilon-transition) */
    if (this->m_state == STATE_INITIAL) {
        this->m_state = STATE_PITCH;
    }

    /* Produce an output for this state */
    switch (this->m_state) {
        /* Initial state, error */
        case STATE_INITIAL: { MBED_ASSERT(false); }
        /* Send pitch */
        case STATE_PITCH: OUT(SPEAKJET_CODE_PITCH)
        case STATE_PITCH_VALUE: OUT(HzToByte(this->m_frequency.current))
        /* Send speed */
        case STATE_SPEED: OUT(SPEAKJET_CODE_SPEED)
        case STATE_SPEED_VALUE: OUT(this->m_speed.current)
        /* Send a byte */
        case STATE_READY: OUT(this->m_source.get())
        case STATE_SPENT: break;
    }    

    /* Do transition */
    switch (this->m_state) {
        /* Initial state, error */
        case STATE_INITIAL: { MBED_ASSERT(false); }
        /* Pitch */
        case STATE_PITCH: GOTO(STATE_PITCH_VALUE)
        case STATE_PITCH_VALUE: GOTO(STATE_READY)
        /* Speed */
        case STATE_SPEED: GOTO(STATE_PITCH_VALUE)
        case STATE_SPEED_VALUE: GOTO(STATE_READY)
        /* Ready */
        case STATE_READY: GOTO(this->m_source.next() ? STATE_READY : STATE_SPENT)
        /* Spent */
        case STATE_SPENT: break;
    }
    
    /* Return true iff we produced something, that is, if we're not spent */
    return !spent;
} 