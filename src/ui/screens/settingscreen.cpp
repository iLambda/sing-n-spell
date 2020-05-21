#include "bitmaps.h"
#include "ui/display.h"
#include "ui/helpers/draw.h"
#include "utils/utils.h"

#include "keymapscreen.h"
#include "settingscreen.h"

#define IF_DIRTY(flag, action)      { if (flag) { (action); } }
#define NEED_SCROLLBAR (utils::size(ui::screen::SettingScreen::m_settingList) > ui::Display::screenHeight())

/* Static members */
ui::screen::SettingScreen::settings_t ui::screen::SettingScreen::m_settings = {0};
const ui::screen::SettingScreen::setting_t ui::screen::SettingScreen::m_settingList[] = {
    {"\x7F Back", nullptr},
    {"Background", &ui::screen::SettingScreen::m_settings.background },
    {"Contrast", &ui::screen::SettingScreen::m_settings.contrast },
};


void ui::screen::SettingScreen::reset(void* state) {
    /* Get self */
    auto self = (SettingScreen*)state;
    /* Initialize */
    self->m_isDirty.value = ~((uint32_t)0);
    self->m_selected = 0;
    /* Set custom characters */
    if (NEED_SCROLLBAR) {
        ui::Display::driver()->createChar(1, bmp::lcd::BMP_LCD_SCROLL);
        ui::Display::driver()->createChar(2, bmp::lcd::BMP_LCD_SCROLL_UP);
        ui::Display::driver()->createChar(3, bmp::lcd::BMP_LCD_SCROLL_DOWN);    
    }
}

void ui::screen::SettingScreen::render(void* state, SerialLCD* display) {
    /* Get self */
    auto self = (SettingScreen*)state;

    /* Draw scrollbars if we need it */
    if (NEED_SCROLLBAR) {
        /* Check if scroll ends dirty */
        if (self->m_isDirty.scrollbarEnd) {
            /* Draw top end */
            display->setCursor(Display::screenWidth() - 1, 0);
            display->writeChar(2);
            /* Draw bottom end */
            display->setCursor(Display::screenWidth() - 1, Display::screenHeight() - 1);
            display->writeChar(3);
        }
        /* Check if scrollbar dirty */
        if (self->m_isDirty.scrollbarMiddle) {
            /* Draw */
            for (uint8_t i = 1; i < Display::screenHeight() - 1; i++) {
                /* Place */
                display->setCursor(Display::screenWidth() - 1, i);
                display->writeChar(1);
            }
        }
    }
    /* Draw each setting */
    for (uint8_t i = 0; i < min((size_t)Display::screenHeight(), utils::size(SettingScreen::m_settingList)); i++) {
        display->setCursor(0, i);
        display->write(SettingScreen::m_settingList[i].text);
    }

    /* Clean dirty inputs */
    self->m_isDirty.value = 0x00;
}

void ui::screen::SettingScreen::update(void* state, bool* dirty) {
    
    /* Wait till the screen is drawn, to avoid a weird race condition */
    Display::dry();
    
    /* Get self */
    auto self = (SettingScreen*)state;

    /* Check if need to flag dirty */
    *dirty = self->m_isDirty.value != 0;
}


void ui::screen::SettingScreen::input(void* state, const io::inputstate_t& inputs, io::outputstate_t& outputs) {
    /* If press, go back */
    if (inputs.buttons.selector) {
        Display::go<KeymapScreen>();
    }
}