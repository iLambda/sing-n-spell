#include "serlcd.h"
#include "bitmaps.h"
#include "io/controller.h"
#include "ui/display.h"
#include "ui/helpers/draw.h"
#include "utils/math.h"

#include "keymapscreen.h"
#include "spellscreen.h"

char kbd_line_buf[ui::Display::screenWidth()+1] = {0};

void ui::screen::SpellScreen::reset(void* state) {
    /* Get self */
    auto self = (SpellScreen*)state;
    /* Reset dirty flags */
    self->m_isDirty.value = ~((uint32_t)0);
    /* Initialize */
    self->m_cursor = {0};
    /* Set custom characters */
    ui::Display::driver()->createChar(1, bmp::lcd::BMP_LCD_SPACE);
    ui::Display::driver()->createChar(2, bmp::lcd::BMP_LCD_BACKSPACE);
}

void ui::screen::SpellScreen::render(void* state, SerialLCD* display) {
    /* Get self */
    auto self = (SpellScreen*)state;

    /* Check if kbd dirty */
    if (self->m_isDirty.keyboard) {
        /* Disable cursor */
        display->noCursor();
        /* Draw the keyboard */
        for (uint8_t j = 0; j < 3; j++) {
            for (uint8_t i = 0; i < ui::Display::screenWidth(); i++) {
                /* Compute index */
                uint8_t index = i + (j * ui::Display::screenWidth());
                /* Spacing */
                if (index % 2) { 
                    kbd_line_buf[i] = ' '; 
                    continue; 
                }
                /* Add characters */
                uint8_t charid = index >> 1;
                switch (charid) {
                    /* This is a special char */
                    case ('Z' - 'A') + 3:
                    case ('Z' - 'A') + 4:
                        kbd_line_buf[i] = charid - ('Z' - 'A') - 2;
                        break;

                    /* This is a letter */
                    default:
                        kbd_line_buf[i] = 'A' + charid;
                        break;
                }
            }
            /* Set cursor to draw line */
            display->setCursor(0, 1 + j);
            display->write(kbd_line_buf);
        }
    }

    /* Enable cursor and set to position */
    display->cursor();
    display->setCursor(self->m_cursor.x * 2, self->m_cursor.y + 1);

    /* Clear */
    self->m_isDirty.value = 0x00;
}

void ui::screen::SpellScreen::update(void* state, bool* dirty) {
    /* Get self */
    // auto self = (SpellScreen*)state;

    *dirty = true;
}

void ui::screen::SpellScreen::input(void* state, const io::inputstate_t& inputs, io::outputstate_t& outputs) {

    /* Get self */
    auto self = (SpellScreen*)state;

    /* Check if alt was pressed */
    if (!inputs.alt) {
        /* Alt not pressed */
        /* Directional keys */
        if (inputs.buttons.next) { self->m_cursor.x = utils::mod((int8_t)self->m_cursor.x + 1, 10); }
        if (inputs.buttons.prev) { self->m_cursor.x = utils::mod((int8_t)self->m_cursor.x - 1, 10); }
        if (inputs.buttons.load) { self->m_cursor.y = utils::mod((int8_t)self->m_cursor.y + 1, 3); }
        if (inputs.buttons.save) { self->m_cursor.y = utils::mod((int8_t)self->m_cursor.y - 1, 3); }
        /* Enter key */
    } else {
        /* Alt is pressed */
        if (inputs.buttons.load) { Display::go<KeymapScreen>(); }
    }
}