#ifndef __SING_N_SPELL_SYNTH_CODE
#define __SING_N_SPELL_SYNTH_CODE

#include <mbed.h>
#include <rtos.h>

#ifdef BUILD_CONFIG_DEBUG
    #include <type_traits>
#endif

#define TTS_CMD_FIRST           synth::tts_cmd_t::TTS_CMD_PAUSE0
#define TTS_CMD_LAST            synth::tts_cmd_t::TTS_CMD_GROUP_END

namespace synth {

    /* A tts command */
    enum tts_cmd_t : uint8_t {
        TTS_CMD_PAUSE0,
        TTS_CMD_PAUSE100,
        TTS_CMD_PAUSE200,
        TTS_CMD_PAUSE700,
        TTS_CMD_WAIT30,
        TTS_CMD_WAIT60,
        TTS_CMD_WAIT90, 
        TTS_CMD_FAST,
        TTS_CMD_SLOW,
        TTS_CMD_STRESS,
        TTS_CMD_RELAX,
        TTS_CMD_REPEAT,
        TTS_CMD_DELAY_10MS,
        TTS_CMD_PITCH,
        TTS_CMD_GROUP_OPEN,
        TTS_CMD_GROUP_END
    };
    /* A tts phoneme */
    enum tts_phoneme_t : uint8_t {
        TTS_PHON_IY = 128, 
        TTS_PHON_IH = 129, 
        TTS_PHON_EY = 130, 
        TTS_PHON_EH = 131, 
        TTS_PHON_AY = 132, 
        TTS_PHON_AX = 133, 
        TTS_PHON_UX = 134, 
        TTS_PHON_OH = 135, 
        TTS_PHON_AW = 136, 
        TTS_PHON_OW = 137, 
        TTS_PHON_UH = 138, 
        TTS_PHON_UW = 139, 
        TTS_PHON_MM = 140, 
        TTS_PHON_NE = 141, 
        TTS_PHON_NO = 142, 
        TTS_PHON_NGE = 143, 
        TTS_PHON_NGO = 144, 
        TTS_PHON_LE = 145, 
        TTS_PHON_LO = 146, 
        TTS_PHON_WW = 147, 
        TTS_PHON_RR = 148, 
        TTS_PHON_IYRR = 149, 
        TTS_PHON_EYRR = 150, 
        TTS_PHON_AXRR = 151, 
        TTS_PHON_AWRR = 152, 
        TTS_PHON_OWRR = 153, 
        TTS_PHON_EYIY = 154, 
        TTS_PHON_OHIY = 155, 
        TTS_PHON_OWIY = 156, 
        TTS_PHON_OHIH = 157, 
        TTS_PHON_IYEH = 158, 
        TTS_PHON_EHLL = 159, 
        TTS_PHON_IYUW = 160, 
        TTS_PHON_AXUW = 161, 
        TTS_PHON_IHWW = 162, 
        TTS_PHON_AYWW = 163, 
        TTS_PHON_OWWW = 164, 
        TTS_PHON_JH = 165, 
        TTS_PHON_VV = 166, 
        TTS_PHON_ZZ = 167, 
        TTS_PHON_ZH = 168, 
        TTS_PHON_DH = 169, 
        TTS_PHON_BE = 170, 
        TTS_PHON_BO = 171, 
        TTS_PHON_EB = 172, 
        TTS_PHON_OB = 173, 
        TTS_PHON_DE = 174, 
        TTS_PHON_DO = 175, 
        TTS_PHON_ED = 176, 
        TTS_PHON_OD = 177, 
        TTS_PHON_GE = 178, 
        TTS_PHON_GO = 179, 
        TTS_PHON_EG = 180, 
        TTS_PHON_OG = 181, 
        TTS_PHON_CH = 182, 
        TTS_PHON_HE = 183, 
        TTS_PHON_HO = 184, 
        TTS_PHON_WH = 185, 
        TTS_PHON_FF = 186, 
        TTS_PHON_SE = 187, 
        TTS_PHON_SO = 188, 
        TTS_PHON_SH = 189, 
        TTS_PHON_TH = 190, 
        TTS_PHON_TT = 191, 
        TTS_PHON_TU = 192, 
        TTS_PHON_TS = 193, 
        TTS_PHON_KE = 194, 
        TTS_PHON_KO = 195, 
        TTS_PHON_EK = 196, 
        TTS_PHON_OK = 197, 
        TTS_PHON_PE = 198, 
        TTS_PHON_PO = 199, 
        TTS_PHON_R0 = 200, 
        TTS_PHON_R1 = 201, 
        TTS_PHON_R2 = 202, 
        TTS_PHON_R3 = 203, 
        TTS_PHON_R4 = 204, 
        TTS_PHON_R5 = 205, 
        TTS_PHON_R6 = 206, 
        TTS_PHON_R7 = 207, 
        TTS_PHON_R8 = 208, 
        TTS_PHON_R9 = 209, 
        TTS_PHON_A0 = 210, 
        TTS_PHON_A1 = 211, 
        TTS_PHON_A2 = 212, 
        TTS_PHON_A3 = 213, 
        TTS_PHON_A4 = 214, 
        TTS_PHON_A5 = 215, 
        TTS_PHON_A6 = 216, 
        TTS_PHON_A7 = 217, 
        TTS_PHON_A8 = 218, 
        TTS_PHON_A9 = 219, 
        TTS_PHON_B0 = 220, 
        TTS_PHON_B1 = 221, 
        TTS_PHON_B2 = 222, 
        TTS_PHON_B3 = 223, 
        TTS_PHON_B4 = 224, 
        TTS_PHON_B5 = 225, 
        TTS_PHON_B6 = 226, 
        TTS_PHON_B7 = 227, 
        TTS_PHON_B8 = 228, 
        TTS_PHON_B9 = 229, 
        TTS_PHON_C0 = 230, 
        TTS_PHON_C1 = 231, 
        TTS_PHON_C2 = 232, 
        TTS_PHON_C3 = 233, 
        TTS_PHON_C4 = 234, 
        TTS_PHON_C5 = 235, 
        TTS_PHON_C6 = 236, 
        TTS_PHON_C7 = 237, 
        TTS_PHON_C8 = 238, 
        TTS_PHON_C9 = 239, 
        TTS_PHON_D0 = 240, 
        TTS_PHON_D1 = 241, 
        TTS_PHON_D2 = 242, 
        TTS_PHON_D3 = 243, 
        TTS_PHON_D4 = 244, 
        TTS_PHON_D5 = 245, 
        TTS_PHON_D6 = 246, 
        TTS_PHON_D7 = 247, 
        TTS_PHON_D8 = 248, 
        TTS_PHON_D9 = 249, 
        TTS_PHON_D10 = 250, 
        TTS_PHON_D11 = 251, 
        TTS_PHON_M0 = 252, 
        TTS_PHON_M1 = 253, 
        TTS_PHON_M2 = 254, 
        TTS_PHON_END = 255
    };
    /* The type of a tts code */
    enum tts_code_type_t : uint8_t {
        TTS_TYPE_PHONEME,
        TTS_TYPE_COMMAND,
        TTS_TYPE_INVALID
    };

    /* Check if value is a command or a phoneme */
    tts_code_type_t tts_code_type(const uint8_t& code);
    /* Go to next value in given range, for a given code */
    void tts_code_next(uint8_t& code);
    /* Go to prev value in given range, for a given code */
    void tts_code_prev(uint8_t& code);
    /* Go to ith value in given range, for a given code */
    void tts_code_delta(uint8_t& code, int8_t delta);
    /* Transform a code into its other type, and return the new code type */
    tts_code_type_t tts_code_transform(uint8_t& code);
    /* Return the instruction for a code */
    const char* tts_code_instruction(const uint8_t& code);
    /* Return the name of a code */
    const char* tts_code_name(const uint8_t& code);

    // /* Ensure code_t is a POD & 8-bit wide*/
    // #ifdef BUILD_CONFIG_DEBUG
    //     static_assert(std::is_pod<tts_cmd_t>::value, "code_t must be a POD !");
    //     static_assert (sizeof(tts_cmd_t) == sizeof(uint8_t), "code_t must be 8-bits wide !");
    // #endif
};

#endif