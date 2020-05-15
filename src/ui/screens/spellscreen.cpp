#include "serlcd.h"
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
                /* Fill the buffer */
                kbd_line_buf[i] = index % 2 ? ' ' : ('A' + (index/2));
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
    auto self = (SpellScreen*)state;

    *dirty = true;
}

void ui::screen::SpellScreen::input(void* state, const io::inputstate_t& inputs, io::outputstate_t& outputs) {
    /* Check if screen */
    if (ui::Display::current() != SpellScreen::getID()) {
        return;
    }

    /* Get self */
    auto self = (SpellScreen*)state;
    /* Check keys */
    if (inputs.buttons.next) { self->m_cursor.x = utils::mod((int8_t)self->m_cursor.x + 1, 10); }
    if (inputs.buttons.prev) { self->m_cursor.x = utils::mod((int8_t)self->m_cursor.x - 1, 10); }
    if (inputs.buttons.load) { self->m_cursor.y = utils::mod((int8_t)self->m_cursor.y + 1, 3); }
    if (inputs.buttons.save) { self->m_cursor.y = utils::mod((int8_t)self->m_cursor.y - 1, 3); }
}