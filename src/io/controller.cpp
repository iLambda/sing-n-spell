#include "controller.h"
#include "midi.h"
#include "pin.h"


Thread io::Controller::m_threadInput;
Thread io::Controller::m_threadMidi;
io::inputstate_t io::Controller::m_inState = {0};
io::inputstate_buttons_t io::Controller::m_buttonsAbsolute = {0};
io::inputstate_buttons_t io::Controller::m_buttonsAbsoluteOld = {0};
io::outputstate_t io::Controller::m_outState = {0};
utils::Event<const io::midimsg_t&> io::Controller::m_eventMidiReceive;
utils::Event<const io::inputstate_t&> io::Controller::m_eventInputReceive;
RawSerial* io::Controller::m_midi = nullptr;

void io::Controller::run() {
    /* Make MIDI */
    //Controller::m_midi = new RawSerial(D1, D0, MIDI_BAUD_RATE);
    /* Start the UI and event threads */
    Controller::m_threadInput.start(callback(&Controller::inputThread));
    Controller::m_threadInput.set_priority(IO_CONTROLLER_THREAD_PRIORITY_INPUT);
}

io::inputstate_t io::Controller::get() {
    /* Return the state */
    return Controller::m_inState;
}

void io::Controller::set(const io::outputstate_t& state) {
    /* Set the state */
    Controller::m_outState = state;
}

void io::Controller::isrMidi(RawSerial* self) {
    /*
     *  CAREFUL !!! This is ISR context
     */
    /* Send data into queue */
    uint8_t* data = Controller::m_midiMail.alloc();
    *data = self->getc();
    /* Put in queue */
    m_midiMail.put(data);
}

void io::Controller::updateButtons() {
    /* Peripherals to read */
    static DigitalIn prev(PINMAP_KBD_PREV, PinMode::PullUp);
    static DigitalIn next(PINMAP_KBD_NEXT, PinMode::PullUp);
    static DigitalIn cmdPhon(PINMAP_KBD_CMDPHON, PinMode::PullUp);
    static DigitalIn individual(PINMAP_KBD_INDIVIDUAL, PinMode::PullUp);
    static DigitalIn alt(PINMAP_KBD_ALT, PinMode::PullUp);
    static DigitalIn prelisten(PINMAP_KBD_PRELISTEN, PinMode::PullUp);
    static DigitalIn load(PINMAP_KBD_LOAD, PinMode::PullUp);
    static DigitalIn save(PINMAP_KBD_SAVE, PinMode::PullUp);
    /* Peripherals to write */
    static BusOut busOut(CTRL_BUS_OUT(PINMAP_KBD_INDIVIDUAL_OUT, PINMAP_KBD_CMDPHON_OUT));


    /* Write output data */
    busOut.write(Controller::m_outState.value);
    
    /* 
        This routine must be 'atomic'.    
        Lock as critical the following code.
        !!! MUST BE SHORT    
    */
    CriticalSectionLock lock;
    
    /* Save for old state */
    Controller::m_buttonsAbsoluteOld = Controller::m_buttonsAbsolute;

    /* Read buttons (!INVERTED) */
    Controller::m_buttonsAbsolute.prev = !prev.read();
    Controller::m_buttonsAbsolute.next = !next.read();
    Controller::m_buttonsAbsolute.cmdphon = !cmdPhon.read();
    Controller::m_buttonsAbsolute.individual = !individual.read();
    Controller::m_buttonsAbsolute.prelisten = !prelisten.read();
    Controller::m_buttonsAbsolute.load = !load.read();
    Controller::m_buttonsAbsolute.save = !save.read();
    /* Read modkeys & stateful toggles */
    Controller::m_inState.alt = !alt.read();
    
    /* Write 'pressed' state for buttons */
    Controller::m_inState.buttons.value = 
            Controller::m_buttonsAbsolute.value
        & (~Controller::m_buttonsAbsoluteOld.value);
}

void io::Controller::midiThread() {
    /* Some data */
    uint8_t payload[2] = {0};
    osEvent mail;
    midimsg_t midimessage;

    /* Thread loop */
    while (1) {
        /* Get the status byte  */
        mail = Controller::m_midiMail.get(osWaitForever);
        /* Get the status byte, and payload size. Free data */
        uint8_t status = *((uint8_t*)mail.value.p);
        int8_t payloadsize = midi_message_len(status) - 1;
        Controller::m_midiMail.free((uint8_t*)mail.value.p);
        /* Reset message, fill in status byte */
        midimessage = {0};
        midimessage.status.value = status;
        /* Get as many payload bytes as needed */
        for (int8_t i = 0; i < payloadsize ; i++) {
            /* Read mail */
            mail = Controller::m_midiMail.get(IO_CONTROLLER_MIDI_MSG_TIMEOUT);
            /* If nothing receive, forget this whole command 
               and go back to waiting for a stat byte */
            if (mail.status != osEventMail) { goto payload_fail; }
            /* Get data and free mail */
            payload[i] = *((uint8_t*)mail.value.p);
            Controller::m_midiMail.free((uint8_t*)mail.value.p);
            /* Increment */
            i++;
        }
        /* Yeet that message */
        midimessage.data.byte1 = payload[0];
        midimessage.data.byte2 = payload[1];
        Controller::m_eventMidiReceive.fire(midimessage);
        /* Loop break label */
        payload_fail:;
    }
}

void io::Controller::inputThread() {
    /* Start MIDI thread and hook interrupt */
    //Controller::m_threadMidi.start(callback(&Controller::midiThread));
    //Controller::m_threadMidi.set_priority(IO_CONTROLLER_THREAD_PRIORITY_MIDI);
    //Controller::m_midi->attach(callback(&Controller::isrMidi, Controller::m_midi));

    /* Thread loop */
    while(1) {
        /* Update buttons */
        updateButtons();
        /* Input received */
        Controller::m_eventInputReceive.fire(Controller::m_inState);
        /* Wait */
        ThisThread::sleep_for(IO_CONTROLLER_REFRESH_INPUT_RATE);
    }
}