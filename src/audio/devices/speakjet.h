#ifndef __SING_N_SPELL_AUDIO_DEVICE_SPEAKJET
#define __SING_N_SPELL_AUDIO_DEVICE_SPEAKJET

#include <mbed.h>
#include <rtos.h>

#include "synth/tts/lexicon.h"

#define SPEAKJET_CMD_PAUSE0         0        ///< Pause 0ms
#define SPEAKJET_CODE_PAUSE1        1        ///< Pause 100ms
#define SPEAKJET_CODE_PAUSE2        2        ///< Pause 200ms
#define SPEAKJET_CODE_PAUSE3        3        ///< Pause 700ms
#define SPEAKJET_CODE_PAUSE4        4        ///< Pause 30ms
#define SPEAKJET_CODE_PAUSE5        5        ///< Pause 60ms
#define SPEAKJET_CODE_PAUSE6        6        ///< Pause 90ms
#define SPEAKJET_CODE_FAST          7        ///< Next phoneme at 0.5 speed
#define SPEAKJET_CODE_SLOW          8        ///< Next phoneme at 1.5 speed
#define SPEAKJET_CODE_STRESS        14       ///< Next phoneme with some stress
#define SPEAKJET_CODE_RELAX         15       ///< Next phoneme with relaxation
#define SPEAKJET_CODE_WAIT          16       ///< Stops and waits for a Start (see manual)
#define SPEAKJET_CODE_SOFT          18       ///< Stops and waits for a Start (see manual)
#define SPEAKJET_CODE_VOLUME        20       ///< Next octet is volume 0 to 127. Default 96
#define SPEAKJET_CODE_SPEED         21       ///< Next octet is speed 0 to 127. Default 114
#define SPEAKJET_CODE_PITCH         22       ///< Next octet is pitch in Hz to 255
#define SPEAKJET_CODE_BEND          23       ///< Next octet is frequency bend  to 15. Default is 5
#define SPEAKJET_CODE_PORTCTR       24       ///< Next octet is port control value. See manual. Default is 7
#define SPEAKJET_CODE_PORT          25       ///< Next octet is Port Output Value. See manual. Default is 0
#define SPEAKJET_CODE_REPEAT        26       ///< Next octet is repeat count. 0 to 255
#define SPEAKJET_CODE_CALLPHRASE    28       ///< Next octet is EEPROM phrase to play and return. See manual.
#define SPEAKJET_CODE_GOTOPHRASE    29       ///< Next octet is EEPROM phgrase to go to. See manual.
#define SPEAKJET_CODE_DELAY         30       ///< Next octet is delay in multiples of 10ms. 0 to 255.
#define SPEAKJET_CODE_RESET         31       ///< Reset Volume Speed, Pitch, Bend to defaults.
#define SPEAKJET_CODE_END           255

#define AUDIO_SPEAKJET_THREAD_PRIORITY             osPriorityHigh
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
