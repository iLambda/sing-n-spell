#ifndef __STM32_INTERNAL_MATH
#define __STM32_INTERNAL_MATH

#include <mbed.h>

namespace utils {   
    
    /* Integer modulus */
    template <class T>
    MBED_FORCEINLINE T mod(const T& a, const T& b) {
        return (a%b+b) % b;
    } 

    /* Rounding a float to an integer */
    MBED_FORCEINLINE int round(float f) {
        return (int)(f + 0.5 - (f < 0));
    }

    /* Rounding a positive float to an unsigned integer */
    MBED_FORCEINLINE unsigned int uround(float f) {
        return (int)(f + 0.5);
    }

}

#endif