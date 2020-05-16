#ifndef __SING_N_SPELL_UI_HELPERS_DRAWTOOLS
#define __SING_N_SPELL_UI_HELPERS_DRAWTOOLS

#include <mbed.h>
#include <serlcd.h>

#include "ui/display.h"

namespace ui {

    /* Draw text centered */
    template <uint8_t N>
    MBED_FORCEINLINE void screen_write_centered(SerialLCD* const& display, const char (&text)[N], uint8_t height) {
        display->setCursor((Display::screenWidth() - N)/2, height);
        display->write(text);
    }

    /* Draw text centered */
    template <uint8_t N>
    MBED_FORCEINLINE void screen_write_centered(SerialLCD* const& display, const char* const& text, uint8_t height) {
        display->setCursor((Display::screenWidth() - (strlen(text)))/2, height);
        display->write(text);
    }

};

#endif