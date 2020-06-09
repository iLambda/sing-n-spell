#ifndef __SING_N_SPELL_MEM_FAIL
#define __SING_N_SPELL_MEM_FAIL

#include <mbed.h>
#include <rtos.h>

namespace mem {

    /* A memory error */
    enum mem_error_t : int8_t {
        /* No error */
        FAIL_MEM_NONE = 0,
        
        /* Memory is full */
        FAIL_MEM_NO_SPACE = -60,
        /* Memory address is wrong */
        FAIL_MEM_ADDR_INVALID = -61,
    };

    /* Convert a memory error to a string */
    const char* mem_fail_explain(const mem_error_t& error);
    /* Copy a memory error to a string */
    void mem_fail_explain(const mem_error_t& error, char* output);
};

#endif