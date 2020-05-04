#include "bootscreen.h"

const uint8_t ui::screen::BootScreen::SCREEN_ID = 0;

ui::screen::BootScreen::BootScreen() {

}

void ui::screen::BootScreen::render(void* state, SerialLCD* display) {
    /* Write */
    display->setCursor(5, 2);
    display->write("Loading...");
}

void ui::screen::BootScreen::update(void* state, bool* dirty) {
    /* Never repaint */
    *dirty = false;
}