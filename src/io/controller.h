#ifndef __SING_N_SPELL_INPUT_CONTROLLER
#define __SING_N_SPELL_INPUT_CONTROLLER

#define IO_CONTROLLER_REFRESH_INPUT_RATE         1

#define IO_CONTROLLER_MIDI_MSG_TIMEOUT           500
#define IO_CONTROLLER_MIDI_QUEUE_SIZE            256

#define IO_CONTROLLER_THREAD_PRIORITY_MIDI       osPriorityNormal
#define IO_CONTROLLER_THREAD_PRIORITY_INPUT      osPriorityNormal
#define IO_CONTROLLER_THREAD_FLAG_MIDI_RECEIVED  0x01

#include <mbed.h>
#include <libmidi.h>
#include <rtos.h>

#include "io/midi.h"
#include "io/state.h"
#include "utils/data/event.h"

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
            static Mail<uint8_t, IO_CONTROLLER_MIDI_QUEUE_SIZE>* m_midiMail;

            /* The midi received event */
            static utils::Event<const midimsg_t&> m_eventMidiReceive;
            /* The input received */
            static utils::Event<const inputstate_t&> m_eventInputReceive;

        private:
            Controller() {}

        public:
            /* Midi receive event. The callbacks are run in midi thread context */
            static const utils::Event<const midimsg_t&>& midiReceive() { return m_eventMidiReceive; }
            /* Input receive event. The callbacks are run in midi thread context */
            static const utils::Event<const inputstate_t&>& inputReceive() { return m_eventInputReceive; }

        public:
            /* Run the controller  */
            static void run();
            /* Get the state */
            __STATIC_FORCEINLINE inputstate_t input() { return Controller::m_inState; }
            /* Return the output state */
            __STATIC_FORCEINLINE outputstate_t& output() { return Controller::m_outState; }
            
        private:
            /* Midi in interrupt */
            static void isrMidi();

            /* Update buttons */
            static void updateButtons();
            /* Send midi msg */
            static void sendMidi(libmidi_event_type event, uint8_t chan, uint8_t data1, uint8_t data2);

            /* The input thread */
            static void inputThread();
            /* The MIDI thread */
            static void midiThread();
    };

};

#endif