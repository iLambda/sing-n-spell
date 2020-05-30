#ifndef __SING_N_SPELL_AUDIO_SOUNDCARD
#define __SING_N_SPELL_AUDIO_SOUNDCARD

#include <mbed.h>

#include "audio/dev/speakjet.h"
#include "audio/dev/tts256.h"
#include "audio/codec/tts2speakjet.h"
#include "synth/tts/lexicon.h"
#include "utils/data/preserved.h"

#define SOUNDCARD_SPEAK_THREAD_PRIORITY          /*osPriorityHigh*/ osPriorityNormal
#define SOUNDCARD_SPEAK_THREAD_FLAG_PLAY         0x01
#define SOUNDCARD_SPEAK_THREAD_FLAG_STOP         0x02
#define SOUNDCARD_SPEAK_THREAD_FLAG_FREQUENCY         0x04


namespace audio {

    class Soundcard {

        private:
            /* The translating chip */
            static dev::TTS256* m_translationChip;
            /* The synthesis chip */
            static dev::SpeakJet* m_soundChip;

            /* The speaking thread */
            static Thread m_speakThread;
            /* The current word mutex */
            static Mutex m_wordMutex;
            
            /* The current word translator */
            static synth::worditerator_t* m_currentWord;
            /* The gate */
            static utils::preserved_t<bool> m_gate;
            /* The frequency */
            static utils::preserved_t<float> m_frequency;

        private:
            Soundcard() { }

        public:
            /* Run the soundcard */
            static void run();
            /* Is the soundcard ready ? */
            static bool ready() {
                return m_soundChip->ready();
            }
            
        public:
            /* Set the current word */
            static void word(const synth::worditerator_t& word);
            static void word(nullptr_t);
            /* Set the gate */
            static void gate(bool value);
            __STATIC_FORCEINLINE bool gate() { return Soundcard::m_gate.current; }
            /* Set the frequency */
            static void frequency(float value);
            __STATIC_FORCEINLINE float frequency() { return Soundcard::m_frequency.current; }
            /* Play the sound, regardless of gate */
            static void play();
            /* Stop all sounds */
            static void shutUp();

        private:   
            /* The speaking thread */
            static void speakThread();

    };
    
}

#endif