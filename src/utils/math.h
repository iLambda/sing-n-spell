#ifndef __STM32_INTERNAL_MATH
#define __STM32_INTERNAL_MATH

#include <mbed.h>

namespace utils {   
    
    /* Integer modulus */
    template <class T>
    MBED_FORCEINLINE T mod(const T& a, const T& b) {
        return (a%b+b) % b;
    }

}

#endif