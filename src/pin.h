#ifndef __SING_N_SPELL_PIN
#define __SING_N_SPELL_PIN

#include <mbed.h>
#include <rtos.h>

/** Screen **/
#define PINMAP_I2C_SDA                  PB_9
#define PINMAP_I2C_SCL                  PB_8

/** Keyboard **/
/* Buttons */
#define PINMAP_KBD_LOAD                 PG_1
#define PINMAP_KBD_SAVE                 PF_9
#define PINMAP_KBD_EDIT                 PF_8
#define PINMAP_KBD_ALT                  PF_2
#define PINMAP_KBD_NEXT                 PE_4
#define PINMAP_KBD_PREV                 PE_5
#define PINMAP_KBD_PRELISTEN            PE_2

/* Lighted toggles */
#define PINMAP_KBD_INDIVIDUAL           PG_4
#define PINMAP_KBD_INDIVIDUAL_OUT       PD_10
#define PINMAP_KBD_CMDPHON              PG_8
#define PINMAP_KBD_CMDPHON_OUT          PE_0

/* Encoders (+/-) */
#define PINMAP_KBD_FILE                 PE_1, PG_0
#define PINMAP_KBD_DATA                 PD_0, PD_1
#define PINMAP_KBD_PITCH                PF_0, PF_1
#define PINMAP_KBD_MENU                 PD_7, PD_6
#define PINMAP_KBD_MENU_CLICK           PE_3

/** Soundchips **/
/* Speakjet */
#define PINMAP_SPEAKJET_TX              NC
#define PINMAP_SPEAKJET_RST             NC
#define PINMAP_SPEAKJET_READY           NC
#define PINMAP_SPEAKJET_SPK             NC


#endif