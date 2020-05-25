#ifndef __SING_N_SPELL_INPUT_GESTURE_DOUBLECLICK
#define __SING_N_SPELL_INPUT_GESTURE_DOUBLECLICK

#include <mbed.h>
#include <rtos.h>

#include "utils/data/event.h"

#define DOUBLE_CLICK_DEFAULT_TIMEOUT        0.400f

namespace io {

    namespace gestures {

        class DoubleClick {

            private: 
                /* The timeout */
                LowPowerTimeout m_timeout;
                /* The timeout duration */
                float m_timeoutDuration;
                /* The state */
                enum state_t : uint8_t {
                    NO_CLICK, SINGLE_CLICK
                } m_state;
                /* The received event */
                utils::Event<bool> m_event;
            
            public:
                /* Create */
                DoubleClick(float duration = DOUBLE_CLICK_DEFAULT_TIMEOUT) {
                    /* Initialize */
                    this->m_timeoutDuration = duration;
                    this->m_state = NO_CLICK;
                }

            private:
                /* The click handler */
                void clickHandler();

            public:
                /* Register a raw click */
                void rawClick();
                /* The click event */
                const utils::Event<bool>& click() { return this->m_event; }
                /* The timeout */
                float& timeout() { return this->m_timeoutDuration; };

        };

    }

}

#endif