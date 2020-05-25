#include "pin.h"
#include "soundcard.h"

audio::dev::TTS256* audio::Soundcard::m_translationChip = nullptr;
audio::dev::SpeakJet* audio::Soundcard::m_soundChip = nullptr;
synth::worditerator_t* audio::Soundcard::m_currentWord = nullptr;  
utils::preserved_t<bool> audio::Soundcard::m_gate = utils::preserved_constexpr(false);
utils::preserved_t<float> audio::Soundcard::m_frequency = utils::preserved_constexpr((float)TTS2SPEAKJET_PITCH_DEFAULT);
Thread audio::Soundcard::m_speakThread;
Mutex audio::Soundcard::m_wordMutex;

synth::worditerator_t worditerator = synth::worditerator_t::null();

void audio::Soundcard::run() {
    /* Setup */
    Soundcard::m_soundChip = new dev::SpeakJet(PINMAP_SPEAKJET_RX, PINMAP_SPEAKJET_RST, PINMAP_SPEAKJET_READY, PINMAP_SPEAKJET_SPK);
    /* Start the UI and event threads */
    Soundcard::m_speakThread.start(callback(&Soundcard::speakThread));
    Soundcard::m_speakThread.set_priority(SOUNDCARD_SPEAK_THREAD_PRIORITY);
}

void audio::Soundcard::speakThread() {
    /* Code data */
    static uint8_t code;
    /* Event loop */
    while(1) {
        /* Wait for play */
        uint32_t flags = ThisThread::flags_wait_any(
                        SOUNDCARD_SPEAK_THREAD_FLAG_PLAY 
                        | SOUNDCARD_SPEAK_THREAD_FLAG_STOP, false);

        /* Check if the detected flag was stop */
        if (flags & SOUNDCARD_SPEAK_THREAD_FLAG_STOP) {
            /* Stop. Clear flag */
            ThisThread::flags_clear(SOUNDCARD_SPEAK_THREAD_FLAG_STOP);
            /* Stopped. Drain and go on*/
            Soundcard::m_soundChip->drain();
        } 
        
        /* Check if the detected flag was play */
        if (flags & SOUNDCARD_SPEAK_THREAD_FLAG_PLAY) {
            /* Play. Clear flag */
            ThisThread::flags_clear(SOUNDCARD_SPEAK_THREAD_FLAG_PLAY);
            /* Lock mutex */
            Soundcard::m_wordMutex.lock();
            /* If word iterator isn't null */
            if (Soundcard::m_currentWord == nullptr) { 
                /* Just Release the mutex */
                Soundcard::m_wordMutex.unlock();
            } else {
                /* Make the translator */
                codec::TTS2Speakjet translator(*m_currentWord);
                translator.frequency() = Soundcard::m_frequency.current;
                /* Release the mutex */
                Soundcard::m_wordMutex.unlock();
                /* While there are elements to play */
                while (translator.next(code)) {
                    /* Check if stop signal received */
                    if (ThisThread::flags_get() & SOUNDCARD_SPEAK_THREAD_FLAG_STOP) {
                        /* Stop putting data in the buffer */
                        break;
                    }
                    /* Send the code */
                    Soundcard::m_soundChip->send(code);
                }
                
            }    
        }    
    }
}


/* Set the current word */
void audio::Soundcard::word(nullptr_t) {
    /* Lock the word */
    m_wordMutex.lock();
    /* Reset word */
    Soundcard::m_currentWord = nullptr;
    /* Stop everything */
    Soundcard::shutUp();
    /* Lock the word */
    m_wordMutex.unlock();
}

/* Set the current word */
void audio::Soundcard::word(const synth::worditerator_t& word) {
    /* If iterators are equal, do nothing */
    if (m_currentWord && *m_currentWord == word) { return; }
    /* Lock the word */
    m_wordMutex.lock();
    /* Copy and restart */
    worditerator = word;
    worditerator.first();
    /* Save current word */
    Soundcard::m_currentWord = &worditerator;
    /* Lock the word */
    m_wordMutex.unlock();
}

/* Set the gate */
void audio::Soundcard::gate(bool value) {
    /* Update gate and send message */
    if (utils::preserved_changes_with(Soundcard::m_gate, value)) {
        /* If gate is now on, send a play msg */
        if (Soundcard::m_gate.current) {
            /* Stop current sound and play */
            Soundcard::m_speakThread.flags_set(
                SOUNDCARD_SPEAK_THREAD_FLAG_STOP 
                | SOUNDCARD_SPEAK_THREAD_FLAG_PLAY);
        }
    }
}

/* Set the frequency */
void audio::Soundcard::frequency(float value) {
    /* Update gate and send message */
    if (utils::preserved_changes_with(Soundcard::m_frequency, value)) {
        /* Edit frequency */
        Soundcard::m_speakThread.flags_set(SOUNDCARD_SPEAK_THREAD_FLAG_FREQUENCY);
        
    }
}

/* Stop all sounds */
void audio::Soundcard::play() {
    /* Stop current sound and play */
    Soundcard::m_speakThread.flags_set(
        SOUNDCARD_SPEAK_THREAD_FLAG_STOP 
        | SOUNDCARD_SPEAK_THREAD_FLAG_PLAY);
}

/* Stop all sounds */
void audio::Soundcard::shutUp() {
    /* Set gate to false */
    utils::preserved_update(Soundcard::m_gate, false);
    /* Stop anyway */
    Soundcard::m_speakThread.flags_set(SOUNDCARD_SPEAK_THREAD_FLAG_STOP);
}