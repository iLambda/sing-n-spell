#include "controller.h"
#include "pin.h"
#include "utils/dev/encoder.h"
#include "utils/debugging.h"

#include "libmidi.h"

Thread io::Controller::m_threadInput;
Thread io::Controller::m_threadMidi;
io::inputstate_t io::Controller::m_inState = {0};
io::inputstate_buttons_t io::Controller::m_buttonsAbsolute = {0};
io::inputstate_buttons_t io::Controller::m_buttonsAbsoluteOld = {0};
io::outputstate_t io::Controller::m_outState = {0};
utils::Event<const io::midimsg_t&> io::Controller::m_eventMidiReceive;
utils::Event<const io::inputstate_t&> io::Controller::m_eventInputReceive;
RawSerial* io::Controller::m_midi = nullptr;
Mail<uint8_t, IO_CONTROLLER_MIDI_QUEUE_SIZE>* io::Controller::m_midiMail = new Mail<uint8_t, IO_CONTROLLER_MIDI_QUEUE_SIZE>();

void io::Controller::run() {
    /* Start the UI and event threads */
    Controller::m_threadInput.start(callback(&Controller::inputThread));
    Controller::m_threadInput.set_priority(IO_CONTROLLER_THREAD_PRIORITY_INPUT);
    /* Make MIDI */
    #ifdef DEBUG_MODE_MIDI_USB 
        Controller::m_midi = &dbg::serial;
    #else
        Controller::m_midi = new RawSerial(NC, PINMAP_DEV_MIDI_IN, MIDI_BAUD_RATE);
    #endif
    Controller::m_midi->attach((&Controller::isrMidi), SerialBase::RxIrq);
    /* Start MIDI thread */
    Controller::m_threadMidi.start(callback(&Controller::midiThread));
    Controller::m_threadMidi.set_priority(IO_CONTROLLER_THREAD_PRIORITY_MIDI);
}

void io::Controller::isrMidi() {
    /*
     *  CAREFUL !!! This is ISR context
     */
    /* Send data into queue */
    uint8_t* data = Controller::m_midiMail->alloc();
    *data = Controller::m_midi->getc();
    /* Put in queue */
    m_midiMail->put(data);
}

void io::Controller::updateButtons() {
    /* Temp data */
    static uint8_t inTmp;
    /* Peripherals */
    static BusOut busOut(CTRL_OUT_BUS(PINMAP_KBD_INDIVIDUAL_OUT, PINMAP_KBD_CMDPHON_OUT));
    static BusIn busInBtn(
        CTRL_IN_BUS_BTN(
            PINMAP_KBD_PRELISTEN, 
            PINMAP_KBD_SAVE, PINMAP_KBD_LOAD,
            PINMAP_KBD_NEXT, PINMAP_KBD_PREV,
            PINMAP_KBD_MENU_CLICK, PINMAP_KBD_INDIVIDUAL, 
            PINMAP_KBD_CMDPHON));
    static DigitalIn btnAlt(PINMAP_KBD_ALT, PinMode::PullUp);    
    static DigitalIn btnEdit(PINMAP_KBD_EDIT, PinMode::PullUp);
    static dev::Encoder fileEncoder(PINMAP_KBD_FILE);
    static dev::Encoder dataEncoder(PINMAP_KBD_DATA);
    static dev::Encoder pitchEncoder(PINMAP_KBD_PITCH);
    static dev::Encoder menuEncoder(PINMAP_KBD_MENU);

    /* Set as pullup */
    busInBtn.mode(PinMode::PullUp);
    /* Save old state */
    Controller::m_buttonsAbsoluteOld = Controller::m_buttonsAbsolute;

    /* Write output data */
    busOut.write(Controller::m_outState.value);

    /* Read bus, and save in variable (reading locks mutexes) */
    inTmp = busInBtn.read();
    /* 
        This routine must be 'atomic'.    
        Lock as critical the following code.
        !!! MUST BE SHORT    
    */
    CriticalSectionLock lock;
    /* Read buttons */
    Controller::m_buttonsAbsolute.value = (~inTmp) & busInBtn.mask();
    /* Read encoders */
    Controller::m_inState.encoders.file = fileEncoder.read();
    Controller::m_inState.encoders.data = dataEncoder.read();
    Controller::m_inState.encoders.pitch = pitchEncoder.read();
    Controller::m_inState.encoders.selector = menuEncoder.read();
    /* Read modkeys & stateful toggles */
    Controller::m_inState.alt = !btnAlt.read();
    Controller::m_inState.edit = !btnEdit.read();
    /* Write 'pressed' state for buttons */
    Controller::m_inState.buttons.value = 
            Controller::m_buttonsAbsolute.value
        & (~Controller::m_buttonsAbsoluteOld.value);
}

void io::Controller::sendMidi(libmidi_event_type event, uint8_t chan, uint8_t data1, uint8_t data2) {
    /* Static data */
    static midimsg_t msg = {0};
    /* Parse message */
    midi_of_libmidi(msg, event, chan, data1, data2);
    /* Fire event */
    Controller::m_eventMidiReceive.fire(msg);
}

void io::Controller::midiThread() {
    /* Some data */
    osEvent mail;
    /* Setup callback */
    libmidi_event_handler(&io::Controller::sendMidi);
    /* Thread loop */
    while (1) {
        /* Get a byte */
        mail = Controller::m_midiMail->get(osWaitForever);
        /* Send it through libmidi */
        libmidi_receive_byte(*((uint8_t*)mail.value.p));
        /* Free the mail */
        Controller::m_midiMail->free((uint8_t*)mail.value.p);
    }
}

void io::Controller::inputThread() {
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