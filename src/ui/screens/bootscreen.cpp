#include "ui/display.h"

#include "keymapscreen.h"
#include "bootscreen.h"

const uint8_t ui::screen::BootScreen::SCREEN_ID = 0;

ui::screen::BootScreen::BootScreen() {
    /* Initialize */
    this->m_frame = 0;
}

void ui::screen::BootScreen::render(void* state, SerialLCD* display) {
    /* Write */
    display->setCursor(5, 2);
    display->write("Loading...");
}

void ui::screen::BootScreen::update(void* state, bool* dirty) {
    /* Get self */
    auto self = (BootScreen*)state;

    /* Wait until time is reached */
    if (self->m_frame * UI_DISPLAY_REFRESH_UI_RATE >= BOOTSCREEN_DURATION) {
        /* Reset time */
        self->m_frame = 0;
        /* Go to keymap screen */
        Display::go(KeymapScreen::getID());
    }
    
    /* Increase time */
    ++self->m_frame;
}