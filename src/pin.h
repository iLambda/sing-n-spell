#ifndef __SING_N_SPELL_PIN
#define __SING_N_SPELL_PIN

#include <mbed.h>
#include <rtos.h>

/** Screen **/
#define PINMAP_I2C_SDA     PB_11
#define PINMAP_I2C_SCL     PB_10

/** Keyboard **/
/* Frame */
#define PINMAP_KBD_PREV                 PE_15
#define PINMAP_KBD_NEXT                 PE_14
#define PINMAP_KBD_CMDPHON              PE_10
#define PINMAP_KBD_CMDPHON_OUT          PE_8
#define PINMAP_KBD_CODE                 NC, NC
/* Note */
#define PINMAP_KBD_PRELISTEN            PG_14
#define PINMAP_KBD_INDIVIDUAL           PE_12
#define PINMAP_KBD_INDIVIDUAL_OUT       PG_9
#define PINMAP_KBD_PITCH                NC, NC
/* Settings */
#define PINMAP_KBD_ALT                  PE_7
#define PINMAP_KBD_EDIT                 NC
/* File */
#define PINMAP_KBD_LOAD                 PF_15
#define PINMAP_KBD_SAVE                 PE_13
#define PINMAP_KBD_FILE                 PE_13
/* Menu */
#define PINMAP_KBD_MENU                 NC, NC
#define PINMAP_KBD_MENU_CLICK           NC

/** Speakjet **/
#define PINMAP_SPEAKJET_TX              NC
#define PINMAP_SPEAKJET_RST             NC
#define PINMAP_SPEAKJET_READY           NC
#define PINMAP_SPEAKJET_SPK             NC


#endif