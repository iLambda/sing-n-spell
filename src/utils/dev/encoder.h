#ifndef __STM32_INTERNAL_DEV_ENCODER
#define __STM32_INTERNAL_DEV_ENCODER

#include <mbed.h>
#include <inttypes.h>

namespace dev {
    
    class Encoder {

        private:
            /* The negative input */
            DigitalIn m_negativeInput;
            /* The positive input */
            DigitalIn m_positiveInput;
            /* The state */
            uint16_t m_state;

        public:
            /* Make an encoder */
            Encoder(PinName positive, PinName negative);

        public:
            /* Read the encoder */
            int8_t read();

    };

}

#endif