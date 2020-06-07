#include "fail.h"

const char* mem_faildesc_oom = "Out of memory";
const char* mem_faildesc_nokaddr = "Invalid address";

/* Convert a memory error to a string */
const char* const& mem::mem_fail_explain(const mem::mem_error_t& error) {
    /* -Wswitch is now error */
    #pragma GCC diagnostic error "-Wswitch"
    /* Check error */
    switch (error) {
        case FAIL_MEM_NONE: return nullptr;
        case FAIL_MEM_NO_SPACE: return mem_faildesc_oom;
        case FAIL_MEM_ADDR_INVALID: return mem_faildesc_nokaddr;
    }
    /* -Wswitch is now warning */
    #pragma GCC diagnostic warning "-Wswitch"
}


void mem::mem_fail_explain(const mem::mem_error_t& error, char* output) {
    strcpy(output, mem_fail_explain(error));
}