#include "speakjet.h"

/* Create a SpeakJet interface */
audio::dev::SpeakJet::SpeakJet(PinName tx, PinName rst, PinName ready, PinName speaking) {
    /* Initialize devices */
    this->m_serial = new RawSerial(tx, NC, 9600);
    this->m_speaking = new InterruptIn(speaking);
    this->m_ready = new InterruptIn(ready);
    this->m_reset = new DigitalOut(rst);

    /* Initialize iterator */
    this->m_word = nullptr;

    /* Set to message thread when SPK pin falls (1 -> 0) */
    this->m_speaking->fall(callback(this, &SpeakJet::onDoneSpeaking));
    /* Set to message thread when RDY pin rises (0 -> 1) */
    this->m_ready->fall(callback(this, &SpeakJet::onReady));

    /* Send a reset signal */
    this->m_reset->write(0);
    ThisThread::sleep_for(100);
    this->m_reset->write(1);

    /* Start thread */
    this->m_speakThread.set_priority(AUDIO_SPEAKJET_THREAD_PRIORITY);
    this->m_speakThread.start(callback(this, &SpeakJet::speakThread));
}

/* On ready */
void audio::dev::SpeakJet::onReady() {
    /* Send ready to thread */
    this->m_speakThread.flags_set(AUDIO_SPEAKJET_THREAD_FLAG_READY);
}

/* On ready */
void audio::dev::SpeakJet::onDoneSpeaking() {
    /* Send ready to thread */
    this->m_speakThread.flags_set(AUDIO_SPEAKJET_THREAD_FLAG_DONE_SPEAK);
}

/* The speak thread */
void audio::dev::SpeakJet::speakThread() {
    /* Wait till chip sends ready */
    ThisThread::flags_wait_all(AUDIO_SPEAKJET_THREAD_FLAG_READY);
    while(1) {
        /* Check if play */    
        ThisThread::flags_wait_all(AUDIO_SPEAKJET_THREAD_FLAG_PLAY);
        /* Check if iterator is null */
        if (this->m_word == nullptr) { continue; }
        /* Lock */
        this->m_wordMutex.lock();
        /* While there are elements available */
        do {
            /* Check if stop signal was received */
            if (ThisThread::flags_get() & AUDIO_SPEAKJET_THREAD_FLAG_STOP) {
                /* Clear it */
                ThisThread::flags_clear(AUDIO_SPEAKJET_THREAD_FLAG_STOP);
                /* Break from loop */
                break;
            }
            /* Wait for done speaking */
            ThisThread::flags_wait_all(AUDIO_SPEAKJET_THREAD_FLAG_DONE_SPEAK);
            /* Get it (TODO : ITERATOR TRANSFORM) */
            uint8_t code = this->m_word->get();
            /* Send it */
            this->m_serial->putc(this->m_word->get());

        } while(this->m_word->next());
        /* Unlock */
        this->m_wordMutex.unlock();
    }
}

/* Speak a single code */
void audio::dev::SpeakJet::speak(const synth::worditerator_t& word) {
    /* Send stop */
    this->m_speakThread.flags_set(AUDIO_SPEAKJET_THREAD_FLAG_STOP);
    /* Wait until acquisition */
    this->m_wordMutex.lock();
    /* Save word */
    if (this->m_word == nullptr) {
        this->m_word = new synth::worditerator_t(word);
    } else {
        *(this->m_word) = word;
    }
    /* Release */
    this->m_wordMutex.unlock();
    /* Signal thread to pick up new word */
    this->m_speakThread.flags_set(AUDIO_SPEAKJET_THREAD_FLAG_PLAY);
}

/* Speak every code in the array, until 0xFF. */
void audio::dev::SpeakJet::stop() {
    /* Signal thread to let go */
    this->m_speakThread.flags_set(AUDIO_SPEAKJET_THREAD_FLAG_STOP);
} 