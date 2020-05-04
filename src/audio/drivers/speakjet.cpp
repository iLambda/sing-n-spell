#include "speakjet.h"

/* Create a SpeakJet interface */
audio::dev::SpeakJet::SpeakJet(PinName tx, PinName rst, PinName ready) {
    /* Initialize devices */
    this->m_serial = new RawSerial(tx, NC, 9600);
    this->m_ready = new DigitalIn(ready);
    this->m_reset = new DigitalOut(rst);

    /* Send a reset signal */
    this->m_reset->write(0);
    ThisThread::sleep_for(100);
    this->m_reset->write(1);
}

/* Speak a single code */
void audio::dev::SpeakJet::SpeakJet::speak(uint8_t code) {
    /* Wait till we're ready */
    this->waitForReady();
    /* Write code */
    this->m_serial->putc(code);
}

/* Speak every code in the array, until 0xFF. */
void audio::dev::SpeakJet::SpeakJet::speak(uint8_t* codes) {
    while (*codes != SPEAKJET_CODE_END) {
        this->speak(*codes++);
    }
}