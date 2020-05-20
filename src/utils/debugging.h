#ifndef __STM32_INTERNAL_DEBUG_UTILS
#define __STM32_INTERNAL_DEBUG_UTILS

#include <mbed.h>
#include <inttypes.h>

namespace dbg {
    
    extern mbed::RawSerial serial;
    extern mbed::BusOut leds;

    void printf(const char* format, ...);

    MBED_FORCEINLINE void put(const char* text) {
        #ifdef BUILD_CONFIG_DEBUG
            dbg::serial.puts(text);
        #endif
    }

    MBED_FORCEINLINE void setLEDs(uint8_t state) {
        /* Set */
        #ifdef BUILD_CONFIG_DEBUG
            dbg::leds.write(state);
        #endif
    }
    
}

#endif