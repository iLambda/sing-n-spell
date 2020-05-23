#ifndef __STM32_INTERNAL_UTILS_PRESERVED
#define __STM32_INTERNAL_UTILS_PRESERVED

#include <rtos.h>
#include <mbed.h>
#include <inttypes.h>
#include <type_traits>

namespace utils {
    
    /* A value, containing its current and last value */
    template <class T>
    struct preserved_t {
        /* The current value */
        T current;
        /* The old value */
        T last;
    };

    /* A constant value */
    template <class T>
    MBED_FORCEINLINE void preserved_constant(preserved_t<T>& target, const T& value) {
        /* Initialize to constant */
        target.current = target.last = value;
    }

    /* A constant value */
    template <class T>
    MBED_FORCEINLINE constexpr preserved_t<T> preserved_constexpr(const T& value) {
        /* Initialize to constant */
        return { value, value };
    }

    template <class T>
    MBED_FORCEINLINE preserved_t<T> preserved_constant(const T& value) {
        /* Initialize to constant */
        return { value, value };
    }

    /* Update a value */
    template <class T>
    MBED_FORCEINLINE void preserved_update(preserved_t<T>& target, const T& value) {
        /* Push back */
        target.last = target.current;
        target.current = value;
    }

    /* Has a value changed ? */
    template <class T>
    MBED_FORCEINLINE bool preserved_has_changed(const preserved_t<T>& target) {
        return target.last != target.current;
    }

    /* Update a value ; has it now changed ? */
    template <class T>
    MBED_FORCEINLINE bool preserved_changes_with(preserved_t<T>& target, const T& value) {
        /* Push back */
        target.last = target.current;
        target.current = value;
        /* Now, has it changed ? */
        return target.last != target.current;
    }

    /* The current is now the last */
    template <class T>
    MBED_FORCEINLINE void preserved_iterate(preserved_t<T>& target) {
        /* Push back */
        target.last = target.current;
    }

}

#endif