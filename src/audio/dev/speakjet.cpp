#include "speakjet.h"
#include "utils/debugging.h"
#include "utils/utils.h"

/* The duration of each sound */
// const uint16_t duration_sound[] = {
//     70, 70, 70, 70, 70, 
//     70, 70, 70, 70, 70, 
//     70, 70, 70, 70, 70, 
//     70, 70, 70, 70, 70, 
//     70, 200, 200, 190, 200, 
//     185, 165, 200, 225, 185, 
//     170, 140, 180, 170, 170, 
//     200, 131, 70, 70, 70, 
//     70 ,70, 45, 45, 10, 
//     10, 45, 45, 10, 10,
//     55, 55, 55, 55, 
//     70, 70, 70, 70, 70,
//     40, 40, 50, 40, 50, 70, 170,
//     55, 55, 55, 45, 99, 99,
//     80, 80, 80, 80, 80,
//     80, 80, 80, 80, 80,
//     300, 101, 102, 540, 530, 500, 
//     135, 600, 300, 250, 
//     200, 270, 280, 260, 300,
//     100, 104, 100, 270, 262, 
//     160, 300, 182, 120, 175, 
//     350, 160, 260, 95, 75,
//     95, 95, 95, 95, 95, 95,
//     95, 95, 95, 95, 95, 95,
//     125, 250, 530, 
//     0
// };
// static_assert (utils::size(duration_sound) == 128);

/* Create a SpeakJet interface */
audio::dev::SpeakJet::SpeakJet(PinName tx, PinName rst, PinName ready, PinName speaking) {
    /* Initialize devices */
    this->m_serial = new RawSerial(tx, NC, 9600);
    this->m_speaking = new DigitalIn(speaking);
    this->m_ready = new InterruptIn(ready);
    this->m_reset = new DigitalOut(rst);
    this->m_isReady = false;

    /* Set to message thread when RDY pin rises (0 -> 1) */
    this->m_ready->fall(callback(this, &SpeakJet::onReady));

    /* Send a reset signal */
    this->m_reset->write(0);
    ThisThread::sleep_for(100);
    this->m_reset->write(1);
}

void audio::dev::SpeakJet::onReady() {
    /* We're ready */
    this->m_isReady = true;
}

void audio::dev::SpeakJet::drain() { 
    /* Flush internal buffer */
    this->m_serial->puts("\\0R\\A");
    /* Send end of sentence */
    this->m_serial->putc(SPEAKJET_CODE_END);
}

bool audio::dev::SpeakJet::send(const uint8_t& code) { 
    /* If buffer full, false */
    if (this->full()) { return false; }
    /* Write */
    this->m_serial->putc(code);
    /* OK */
    return true;          
}

/* The buffer thread */
// void audio::dev::SpeakJet::bufferThread() {
//     /* Data */
//     timed_code_t code;
//     /* Wait till chip sends ready */
//     ThisThread::flags_wait_all(AUDIO_SPEAKJET_THREAD_FLAG_READY);
//     /* Lock to avoid race condition */
//     this->m_bufferMutex.lock();
//     /* Go next */
//     while(1) {
//         /* Unlock to avoid race condition */
//         this->m_bufferMutex.unlock();
//         /* Wait for buffer nonempty, but don't clear */
//         ThisThread::flags_wait_all(AUDIO_SPEAKJET_THREAD_FLAG_BUFFER_NONEMPTY, false);
//         /* While buffer has elements */
//         while (this->m_buffer.pop(code)) {
//             /* Send it */
//             this->m_serial->putc(code.data);
//             /* Wait for designated duration */
//             if (code.duration > AUDIO_SPEAKJET_BUFFER_PREPARATION_TIME_MS) {
//                 ThisThread::sleep_for(code.duration - 10);
//             }
//         }
//         /* Lock to avoid race condition */
//         this->m_bufferMutex.lock();
//         /* Buffer has no more elements. Clean the flag */
//         ThisThread::flags_clear(AUDIO_SPEAKJET_THREAD_FLAG_BUFFER_NONEMPTY);
//     }
// }