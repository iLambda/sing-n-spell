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
                DigitalIn* m_speaking;
                /* Is the chip ready ? */
                bool m_isReady;

            public:
                /* Create a SpeakJet interface */
                SpeakJet(PinName tx, PinName rst, PinName ready, PinName speaking);

            private:
                /* On ready interrupt */
                void onReady();
                
            public:
                /* Is the chip ready */
                MBED_FORCEINLINE bool ready() { return this->m_isReady; }
                /* Is the chip speaking */
                MBED_FORCEINLINE bool speaking() { return !!this->m_speaking->read(); }
                /* Is the buffer full ? */
                MBED_FORCEINLINE bool full() { return false; }
                
                /* Empty the whole buffer */
                void drain();
                /* Add a value to the buffer */
                bool send(const uint8_t& code);
        };

    }

}

#endif
