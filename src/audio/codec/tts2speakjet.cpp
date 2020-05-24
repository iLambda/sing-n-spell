#include "audio/dev/speakjet.h"
#include "tts2speakjet.h"

/* Create a translator */
audio::codec::TTS2Speakjet::TTS2Speakjet(const synth::worditerator_t& it) : m_source(it) {
    /* Initialize */
    this->m_initialPosition = this->m_source.position();
    utils::preserved_constant<uint8_t>(this->m_frequency, TTS2SPEAKJET_PITCH_DEFAULT);
    utils::preserved_constant<uint8_t>(this->m_speed, TTS2SPEAKJET_SPEED_DEFAULT);
    /* Reset the state */
    this->reset();
}

/* Reset the current translator state */
void audio::codec::TTS2Speakjet::reset() {
    /* Reset iterator to initial position */
    this->m_source.select(this->m_initialPosition);
    this->m_spent = false;
    /* Say we're in initial state */
    this->m_state = STATE_INITIAL;
}

/* Apply transition */
MBED_FORCEINLINE void nextState() {
    /* Initial state */
}


/* Get next produced code. Returns amount of codes left to be produced by current word command. */
bool audio::codec::TTS2Speakjet::next(uint8_t& out) {

    /* Macro */
    // #define OUT(c, d) { output = (c); duration = (d); break; }

    // /* Produce for state */
    // switch (this->m_state) {
    //     /* Send pitch */
    //     case STATE_PITCH: OUT(SPEAKJET_CODE_PITCH, 0)
    //     case STATE_PITCH_VALUE: OUT(this->m_frequency.current, 0)
    //     /* Send speed */
    //     case STATE_SPEED: OUT(SPEAKJET_CODE_SPEED, 0)
    //     case STATE_SPEED_VALUE: OUT(this->m_speed.current, 0)
    // }    

    // /* Do transition */
    // nextState();

    // /* If we're in now ready state */
    // return this->m_state != STATE_READY;

    if (!m_spent) {
        out = m_source.get();
        if (!m_source.next()) { m_spent = true; }
        if (out == 0xFF) { m_spent = true; }
        return true;
    }
    
    return false;
} 