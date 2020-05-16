#ifndef __SING_N_SPELL_MEM_ERROR
#define __SING_N_SPELL_MEM_ERROR

#include <mbed.h>
#include <rtos.h>

namespace mem {

    /* A memory error */
    enum mem_error_t : signed int {
        /* No error */
        ERROR_MEM_NONE = 0,
        
        /* Memory is full */
        ERROR_MEM_NO_SPACE = -60,
        /* Memory address is wrong */
        ERROR_MEM_ADDR_INVALID = -61,
    };

    /* Convert a memory error to a string */
    const char* const& mem_error_explain(const mem_error_t& error) { return ""; }
    /* Write a memory error to a string */
    void mem_error_explain(const mem_error_t& error, char* output) {
        /* Output */
        // const char* explanation = mem_error_explain(error);
        // /* Copy to */
        // strcpy();
    }
};

#endif