#ifndef __SING_N_SPELL_AUDIO_CODEC_TTS2SPEAKJET
#define __SING_N_SPELL_AUDIO_CODEC_TTS2SPEAKJET

#include <mbed.h>
#include <rtos.h>

#include "synth/tts/code.h"
#include "synth/tts/lexicon.h"
#include "utils/data/preserved.h"

#define TTS2SPEAKJET_PITCH_DEFAULT      88
#define TTS2SPEAKJET_SPEED_DEFAULT      114

namespace audio {
    
    namespace codec {

        class TTS2Speakjet {

            private:
                /* The state */
                enum : uint8_t {
                    STATE_INITIAL,
                    STATE_PITCH,
                    STATE_PITCH_VALUE,
                    STATE_SPEED,
                    STATE_SPEED_VALUE,
                    STATE_READY,
                    
                } m_state;
                /* The frequency */
                utils::preserved_t<uint8_t> m_frequency;
                /* The speed */
                utils::preserved_t<uint8_t> m_speed;

                /* Is the iterator spent ? */
                bool m_spent;

                /* The word iterator */
                synth::worditerator_t m_source;
                /* The initial position */
                size_t m_initialPosition;

            public:
                /* Create a translator */
                TTS2Speakjet(const synth::worditerator_t& it);

            public:
                /* Get the underlying word iterator */
                MBED_FORCEINLINE const synth::worditerator_t& iterator() { return m_source; }
                /* Get the underlying word */
                MBED_FORCEINLINE const synth::word_t* word() { return m_source.word(); }

                /* Get next produced code. Returns true if it wrote a code. */
                bool next(uint8_t& output); 
                /* Reset the current translator state */
                void reset();

            public:
                /* Set frequency */
                MBED_FORCEINLINE uint8_t& frequency() { return m_frequency.current; }
                /* Set frequency */
                MBED_FORCEINLINE uint8_t& speed() { return m_speed.current; }
        };

    }

}

#endif
