#ifndef __SING_N_SPELL_MEM_MAP
#define __SING_N_SPELL_MEM_MAP

#include <mbed.h>
#include <rtos.h>

namespace mem {

    /* Settings */
    #define MMAP_SETTING_BACKGROUND         0x0000
    #define MMAP_SETTING_CONTRAST           0x0001
    #define MMAP_SETTING_KEY_REPEAT         0x0002
    
    /* Keymap */
    #define MMAP_KEYMAPS_BEGIN              0x20000
    #define MMAP_KEYMAPS_STRIDE             0x02000 //128*64 
    #define MMAP_KEYMAPS_COUNT              16


};

#endif