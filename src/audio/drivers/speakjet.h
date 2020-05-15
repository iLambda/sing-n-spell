#ifndef __SING_N_SPELL_AUDIO_DEVICE_SPEAKJET
#define __SING_N_SPELL_AUDIO_DEVICE_SPEAKJET

#include <mbed.h>
#include <rtos.h>

#include "synth/tts/lexicon.h"

#define SPEAKJET_CODE_END       0xFF

#define AUDIO_SPEAKJET_THREAD_PRIORITY       osPriorityHigh
#define AUDIO_SPEAKJET_THREAD_FLAG_STOP            0x01
#define AUDIO_SPEAKJET_THREAD_FLAG_PLAY            0x02
#define AUDIO_SPEAKJET_THREAD_FLAG_READY           0x04
#define AUDIO_SPEAKJET_THREAD_FLAG_DONE_SPEAK      0x08

namespace audio {
    
    namespace dev {

        class SpeakJet {

            private:
                /* The serial object */
                RawSerial* m_serial;
                /* The reset pin */
                DigitalOut* m_reset;
                /* The ready pin */
                InterruptIn* m_ready;
                /* The speaking pin */
                InterruptIn* m_speaking;

                /* The current word */
                synth::worditerator_t* m_word;
                /* The word mutex */
                Mutex m_wordMutex;

                /* The speaking thread */
                Thread m_speakThread;

            public:
                /* Create a SpeakJet interface */
                SpeakJet(PinName tx, PinName rst, PinName ready, PinName speaking);

            private:
                /* The speak thread */
                void speakThread();
                /* On ready interrupt */
                void onReady();
                /* On done speaking interrupt */
                void onDoneSpeaking();
                
            public:
                /* Speak a word */
                void speak(const synth::worditerator_t& word);
                /* Stop speaking */
                void stop();
        };

    }

}

#endif
