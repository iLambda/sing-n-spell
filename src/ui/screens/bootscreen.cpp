#include "audio/soundcard.h"
#include "ui/display.h"
#include "ui/helpers/draw.h"
#include "version.h"

#include "keymapscreen.h"
#include "bootscreen.h"

void ui::screen::BootScreen::reset(void* state) {
    /* Get self */
    auto self = (BootScreen*)state;
    /* Initialize */
    self->m_frame = 0;
}

void ui::screen::BootScreen::render(void* state, SerialLCD* display) {
    /* Write */
    screen_write_centered(display, "Sing'n'Spell", 1);
    screen_write_centered(display, BUILD_VERSION_NBUILD, 2);
}

void ui::screen::BootScreen::update(void* state, bool* dirty) {
    /* Get self */
    auto self = (BootScreen*)state;

    /* Wait until time is reached */
    if (self->m_frame * UI_DISPLAY_REFRESH_UI_RATE >= BOOTSCREEN_DURATION) {
        /* Check if sound chip ready */
        if (audio::Soundcard::ready()) {
            /* Go to keymap screen */
            Display::go<KeymapScreen>();
        }
    } else {
        /* Increase time */
        ++self->m_frame;
    }
}