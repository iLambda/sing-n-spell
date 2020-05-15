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

}

#endif