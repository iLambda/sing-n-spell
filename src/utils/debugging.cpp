#include "debugging.h"

mbed::RawSerial dbg::serial(USBTX, USBRX, 31250);
mbed::BusOut dbg::leds(LED1, LED2, LED3);

void dbg::printf(const char* format, ...) {
    /* Send to pc */
    va_list args;
    va_start(args, format);
    dbg::serial.vprintf(format, args);
    va_end(args);
}