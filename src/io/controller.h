#ifndef __SING_N_SPELL_INPUT_CONTROLLER
#define __SING_N_SPELL_INPUT_CONTROLLER

#define IO_CONTROLLER_REFRESH_INPUT_RATE         1

#define IO_CONTROLLER_MIDI_MSG_TIMEOUT           500
#define IO_CONTROLLER_MIDI_QUEUE_SIZE            256

#define IO_CONTROLLER_THREAD_PRIORITY_MIDI       osPriorityNormal
#define IO_CONTROLLER_THREAD_PRIORITY_INPUT      osPriorityNormal
#define IO_CONTROLLER_THREAD_FLAG_MIDI_RECEIVED  0x01

#include <mbed.h>
#include <rtos.h>

#include "io/midi.h"
#include "io/state.h"
#include "utils/event.h"

namespace io {

    class Controller {

        private:
            /* The midi serial controller */
            static RawSerial* m_midi;

            /* The buttons state */
            static inputstate_buttons_t m_buttonsAbsolute;
            /* The buttons state */
            static inputstate_buttons_t m_buttonsAbsoluteOld;
            /* The state */
            static inputstate_t m_inState;
            /* Output state*/
            static outputstate_t m_outState;
            /* The input thread */
            static Thread m_threadInput;
            /* The MIDI thread */
            static Thread m_threadMidi;

            /* The midi queue */
            static Mail<uint8_t, IO_CONTROLLER_MIDI_QUEUE_SIZE> m_midiMail;

            /* The midi received event */
            static utils::Event<midimsg_t> m_eventMidiReceive;
            /* The input received */
            static utils::Event<const inputstate_t&> m_eventInputReceive;

        private:
            Controller() {}

        public:
            /* Midi receive event. The callbacks are run in midi thread context */
            static const utils::Event<midimsg_t>& midiReceive() { return m_eventMidiReceive; }
            /* Input receive event. The callbacks are run in midi thread context */
            static const utils::Event<const inputstate_t&>& inputReceive() { return m_eventInputReceive; }

        public:
            /* Run the controller  */
            static void run();
            /* Get the state */
            static inputstate_t get();
            /* Sets the state */
            static void set(const outputstate_t& state);
            
        private:
            /* Midi in interrupt */
            static void isrMidi(RawSerial* self);

            /* Update buttons */
            __STATIC_FORCEINLINE void updateButtons();

            /* The input thread */
            static void inputThread();
            /* The MIDI thread */
            static void midiThread();
    };

};

#endif