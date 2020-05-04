#ifndef __SING_N_SPELL_AUDIO_DEVICE_SPEAKJET
#define __SING_N_SPELL_AUDIO_DEVICE_SPEAKJET

#include <mbed.h>
#include <rtos.h>

#define SPEAKJET_CODE_END       0xFF

namespace audio {
    
    namespace dev {

        class SpeakJet {

            private:
                /* The serial object */
                RawSerial* m_serial;
                /* The reset pin */
                DigitalOut* m_reset;
                /* The ready pin */
                DigitalIn* m_ready;

            public:
                /* Create a SpeakJet interface */
                SpeakJet(PinName tx, PinName rst, PinName ready);

            private:
                /* Is the device ready ? */
                MBED_FORCEINLINE bool isReady() { return !this->m_ready->read(); }
                /* Wait until ready */
                MBED_FORCEINLINE void waitForReady() { 
                    while (!this->isReady()) {
                        ThisThread::sleep_for(1);
                    } 
                }
                
            public:
                /* Speak a single code */
                void speak(uint8_t code);
                /* Speak every code in the array, until 0xFF. */
                void speak(uint8_t* codes);
        };

    }

}

#endif
