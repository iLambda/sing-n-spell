#ifndef __STM32_INTERNAL_STR
#define __STM32_INTERNAL_STR

#include <mbed.h>

namespace utils {   
    
    /* Copy a string without null terminator (dst must be at least of size n+1) */
    MBED_FORCEINLINE void ncopy_then_pad(char* dst, const char* src, char pad, size_t n) {
        size_t i = 0;
        /* For all the array */
        while (i++ < n) { 
            /* Set current char to be string if it is available ; else, set pad character */
            *(dst++) = *src ? *src++ : pad; 
        }
        /* Zero term */
        dst[n] = '\0';
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