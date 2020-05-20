#include "encoder.h"

#define R_START 0x0
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5

#define DIR_NONE 0x0
#define DIR_CW 0xFF0
#define DIR_CCW 0x010

const uint16_t ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};

dev::Encoder::Encoder(PinName positive, PinName negative) : 
    m_negativeInput(negative, PinMode::PullUp),
    m_positiveInput(positive, PinMode::PullUp), 
    m_state(R_START)
{ 
    
}

int8_t dev::Encoder::read() {
    /* Read both */
    auto neg = this->m_negativeInput.read();
    auto pos = this->m_positiveInput.read();
    /* Make transition */
    auto transition = (neg << 1) | pos;
    /* Move */
    m_state = ttable[m_state & 0x0F][transition];
    /* Return */
    return (int8_t)((m_state & 0xFF0) >> 4);
}