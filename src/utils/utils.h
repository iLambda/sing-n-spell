#ifndef __STM32_INTERNAL_UTILS
#define __STM32_INTERNAL_UTILS

#include <mbed.h>

namespace utils {
    
    /* Get array size */
    template <class T, size_t N>
    constexpr size_t size(const T (&array)[N]) noexcept {
        return N;
    }

    /* Convert uint8_t into non 0 term string */
    MBED_FORCEINLINE void uint8_hex_to_str(char* const& dest, const uint8_t& value) {
        uint quad0, quad1;
        /* Convert */
        dest[0] = (quad0 = ((value >> 4) & 0x0F)) >= 0x0A ? (('A' - 0x0A) + quad0) : ('0' + quad0);
        dest[1] = (quad1 = (value & 0x0F))        >= 0x0A ? (('A' - 0x0A) + quad1) : ('0' + quad1);
    }
    
    /* Convert uint8_t into 0 termed string */
    MBED_FORCEINLINE void uint8_hex_to_cstr(char* const& dest, const uint8_t& value) {
        uint8_hex_to_str(dest, value);
        dest[2] = '\0';
    }
}

#endif