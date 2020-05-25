#ifndef __STM32_INTERNAL_UTILS_CHRONO
#define __STM32_INTERNAL_UTILS_CHRONO

#include <rtos.h>
#include <mbed.h>
#include <inttypes.h>
#include <type_traits>

namespace utils {
    
    /* A chronometer */
    struct chrono_t {
        /* The stamp */
        uint64_t stamp;
    };

    /* Initialize chronometer */
    MBED_FORCEINLINE constexpr chrono_t chrono_reset() {
        return {0};
    }

    /* Initialize chronometer */
    MBED_FORCEINLINE void chrono_reset(chrono_t& chrono) {
        chrono.stamp = 0;
    }

    /* Save time */
    MBED_FORCEINLINE uint64_t chrono_lap(chrono_t& chrono) {
        return chrono.stamp = get_ms_count();
    }

    /* Get elapsed time time */
    MBED_FORCEINLINE uint64_t chrono_elapsed(const chrono_t& chrono) {
        return get_ms_count() - chrono.stamp;
    }
}

#endif