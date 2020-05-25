#include "doubleclick.h"

void io::gestures::DoubleClick::clickHandler() {
    /* Reset state */
    this->m_state = NO_CLICK;
    /* Timed out : SINGLE CLICK*/
    this->m_event.fire(false);
}

void io::gestures::DoubleClick::rawClick() {
    /* Check state */
    switch (this->m_state) {
        /* Register a click and attach handler */
        case NO_CLICK: {
            /* Set in state single click */
            this->m_state = SINGLE_CLICK;
            /* Make the timeout */
            m_timeout.attach(callback(this, &DoubleClick::clickHandler), this->m_timeoutDuration);
            break;
        }
        /* Detach handler and register double click */
        case SINGLE_CLICK: {
            /* Make the timeout */
            m_timeout.detach();
            /* Set in state single click */
            this->m_state = NO_CLICK;
            /* Fire */
            this->m_event.fire(true);
            break;
        }
    }
}