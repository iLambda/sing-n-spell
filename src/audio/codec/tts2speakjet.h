#ifndef __SING_N_SPELL_AUDIO_CODEC_TTS2SPEAKJET
#define __SING_N_SPELL_AUDIO_CODEC_TTS2SPEAKJET

#include <mbed.h>
#include <rtos.h>

#include "synth/tts/code.h"
#include "synth/tts/lexicon.h"
#include "utils/data/preserved.h"

#define TTS2SPEAKJET_PITCH_DEFAULT      88.0f
#define TTS2SPEAKJET_SPEED_DEFAULT      114

namespace audio {
    
    namespace codec {

        class TTS2Speakjet {

            private:
                /* The state */
                enum : uint8_t {
                    STATE_INITIAL,
                    STATE_PITCH,
                    STATE_SPEED,
                    STATE_READY,
                    STATE_SPENT,
                    STATE_DATA_PHONEME,
                    STATE_DATA_SKIP,
                    STATE_DATA_CMD_TRANS,
                    
                } m_state;
                /* The state of current long command */
                bool m_longCmd;
                /* The start of the sustain command */
                size_t m_sustainStartIndex;
                /* The end of the sustain command */
                size_t m_sustainEndIndex;
                /* Are we sustaining the zone ? */
                bool m_isSustaining;


                /* The frequency */
                utils::preserved_t<float> m_frequency;
                /* The speed */
                utils::preserved_t<uint8_t> m_speed;
                /* The sustain mode enable */
                bool m_sustainEnable;

                /* The word iterator */
                synth::worditerator_t m_source;
                /* The initial position */
                size_t m_initialPosition;

            public:
                /* Create a translator */
                TTS2Speakjet(const synth::worditerator_t& it, float frequency = TTS2SPEAKJET_PITCH_DEFAULT, uint8_t speed = TTS2SPEAKJET_SPEED_DEFAULT, bool sustain = false);

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
                MBED_FORCEINLINE float& frequency() { return m_frequency.current; }
                /* Set frequency */
                MBED_FORCEINLINE uint8_t& speed() { return m_speed.current; }
                /* Set frequency */
                MBED_FORCEINLINE bool& sustain() { return m_sustainEnable; }
        };

    }

}

#endif
