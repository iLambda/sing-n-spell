#include "bitmaps.h"
#include "audio/soundcard.h"
#include "io/controller.h"
#include "io/midi.h"
#include "synth/engine.h"
#include "synth/tts/code.h"
#include "ui/display.h"
#include "ui/screens/spellscreen.h"
#include "ui/screens/settingscreen.h"
#include "utils/string.h"
#include "utils/utils.h"

#include "keymapscreen.h"

#define IF_DIRTY(flag, action)      { if (flag) { (action); } }

/* Constant strings (!!!PADDED!!!) */
const char* const cstr_mode[] = { 
    "Global", 
    "Local " 
};
const char* const cstr_param[][2] = { 
    {"Transpose", "Pitch    "}, 
    {"Speed    ", "Speed    "}     
};
const char* const cstr_datamode_brk[] = {
    "<>", "[]"
};
const char* cstr_invalid_instruction = "??";
/* Buffers */
uint8_t frm_line[ui::Display::screenWidth() + 1] = {0};
uint8_t frm_cmd[ui::Display::screenWidth() + 1] = {0};
uint8_t frm_buf_before[6] = {0};
uint8_t frm_buf_after[1] = {0};
char str_data_pos[] = "--/--";
char str_data_name[ui::Display::screenWidth() - utils::size(str_data_pos) + 1] = {0};
char str_buf_note[5] = {0};
char str_buf_itoa[4] = {0};
char str_invalid[] = "Invalid (  )";

ui::screen::KeymapScreen::KeymapScreen() : 
    m_workbench(utils::preserved_constant(synth::worditerator_t::null())) {
        this->m_prelistenBench = nullptr; 

    /* Make gestures */
    m_gestures.altNext.timeout() = DOUBLE_CLICK_DEFAULT_TIMEOUT;
    m_gestures.altPrev.timeout() = DOUBLE_CLICK_DEFAULT_TIMEOUT;
    /* Register */
    m_gestures.altNext.click() += callback(this, &ui::screen::KeymapScreen::onAltNextClick);
    m_gestures.altPrev.click() += callback(this, &ui::screen::KeymapScreen::onAltPrevClick);
}


void ui::screen::KeymapScreen::reset(void* state) {
    /* Get self */
    auto self = (KeymapScreen*)state;
    /* Initialize */
    self->m_isDirty.value = ~((uint32_t)0);
    self->m_parameter = utils::preserved_constant(PARAM_PITCH);
    self->m_paramTarget = utils::preserved_constant(&synth::Engine::key().pitch);
    self->m_paramValue = utils::preserved_constant(synth::Engine::key().pitch);
    self->m_datamode = utils::preserved_constant(synth::TTS_TYPE_PHONEME);
    self->m_note = utils::preserved_constant(synth::Engine::note());
    self->m_keymode = utils::preserved_constant(synth::Engine::key().mode);
    self->m_workbench = utils::preserved_constant(synth::Engine::workbenchIterator());
    self->m_workbenchValueChanged = false;
    
    /* Allocate prelisten bench if needed */
    if (!self->m_prelistenBench) {
        bool res = synth::Lexicon::alloc(self->m_prelistenBench, ENGINE_WORKBENCH_WORD_SIZE);
        MBED_ASSERT(res);
    }
    /* Add custom characters */
    ui::Display::driver()->createChar(6, bmp::lcd::BMP_LCD_TIMES);
    ui::Display::driver()->createChar(7, bmp::lcd::BMP_LCD_DELTA);
}

/*******************************************
 * Helpers
 *******************************************/

MBED_FORCEINLINE ui::screen::KeymapScreen::DataMode map_datamode(const synth::tts_code_type_t& ty) {
    switch (ty) {
        case synth::TTS_TYPE_COMMAND: return ui::screen::KeymapScreen::DATAMODE_CMD;
        case synth::TTS_TYPE_PHONEME: return ui::screen::KeymapScreen::DATAMODE_PHN;
        case synth::TTS_TYPE_INVALID: return ui::screen::KeymapScreen::DATAMODE_PHN;
    }
    MBED_ASSERT(false);
}

/*******************************************
 * Draw functions
 *******************************************/

/* Draw the note */
MBED_FORCEINLINE void drawNote(SerialLCD* const& display, uint8_t note) {
    /* Draw note */
    uint8_t len = io::midi_note_to_cstr(str_buf_note, note);
    /* Write it */
    display->setCursor(ui::Display::screenWidth() - 3, 0);
    display->write(" ");
    display->setCursor(ui::Display::screenWidth() - len, 0);
    display->write(str_buf_note);
}

/* Draw the mode on screen */
MBED_FORCEINLINE void drawMode(SerialLCD* const& display, synth::keymode_t mode) {
    /* Write it */
    display->setCursor(0, 0);
    display->write(cstr_mode[mode]);
}

/* Draw the parameter name on screen */
MBED_FORCEINLINE void drawParamName(SerialLCD* const& display, synth::keymode_t mode, ui::screen::KeymapScreen::Parameter param) {
    /* Write it */
    display->setCursor(0, 1);
    display->write(cstr_param[param][mode]);
}

/* Draw the parameter name on screen */
MBED_FORCEINLINE void drawParamValue(SerialLCD* const& display, synth::keymode_t mode, ui::screen::KeymapScreen::Parameter param, uint8_t* src) {
    /* Place cursor */
    display->setCursor(ui::Display::screenWidth() - 3, 1);
    display->write(" ");
    /* If pointer not null, write */
    if (src != nullptr) {
        /* Check which parameter this is */
        switch (param) {
            /* Render pitch as a note */
            case ui::screen::KeymapScreen::PARAM_PITCH:
                /* If this is local, this is pitch : render as note */
                if (mode == synth::KEY_MODE_LOCAL) {
                    /* Draw note */
                    uint8_t len = io::midi_note_to_cstr(str_buf_note, *src);
                    /* Write it */
                    display->setCursor(ui::Display::screenWidth() - len, 1);
                    display->write(str_buf_note);
                    /* Done ! */
                    break;
                }
                /* Fallthrough */
            default:
                /* Write number */
                utils::uint8_hex_to_cstr(str_buf_itoa, *src);
                display->write(str_buf_itoa);
                break;
        }
    } else {
        /* Write no value */
        display->write("--");
    }
}

MBED_FORCEINLINE uint8_t drawPhon(uint8_t* const& screenbuf, const int& x, const uint8_t& phon) {
    /* Check */
    if (x < 0) return 0;
    /* Get string for given code */
    auto codename = synth::tts_code_instruction(phon);
    if (codename == nullptr) { codename = cstr_invalid_instruction; }
    /* Compute its length */
    int len = strlen(codename);
    /* While there's space */
    int i = 0;
    for (i = 0; i < min(len, x+1); i++) {
        /* Write the current char */
        screenbuf[x - i] = codename[len - i - 1];
    }
    /* Return progess */
    return ((uint8_t)i);
}

/* Draw the current data */
MBED_FORCEINLINE void drawData(SerialLCD* const& display, const synth::worditerator_t& it) {
    /* Write frame to buffer */
    utils::uint8_hex_to_str(&str_data_pos[0], it.position() + 1);
    utils::uint8_hex_to_str(&str_data_pos[3], it.length());
    /* Get name for current value */
    auto name = synth::tts_code_name(it.get());
    /* Check if valid */
    if (name != nullptr) {
        /* Write name of command to buffer */
        utils::ncopy_then_pad(str_data_name, name, ' ',  utils::size(str_data_name)-1);    
    } else {
        /* Write code in invalid buffer */
        utils::uint8_hex_to_str(&str_invalid[9], it.get());
        /* Write name of command to buffer */
        utils::ncopy_then_pad(str_data_name, str_invalid, ' ',  utils::size(str_data_name)-1); 
    }
    
    /* Print it */
    display->setCursor(0, 2);
    display->write(str_data_name);
    display->write(' ');
    display->write(str_data_pos);
}

/* Draw the frame */
MBED_FORCEINLINE void drawFrame(SerialLCD* const& display, const bool& wholeFrame, const ui::screen::KeymapScreen::DataMode &datamode, const synth::worditerator_t& it) {
    /* The cursor */
    int x = ui::Display::screenWidth() - 1;
    /* Draw the current cmd/phon */
    frm_line[x--] = cstr_datamode_brk[datamode][1];
    x -= drawPhon(frm_line, x, it.get());
    frm_line[x--] = cstr_datamode_brk[datamode][0];

    /* If need to redraw whole frame */
    if (wholeFrame) {
        /* Get context */
        size_t before, after;
        it.context(
            utils::size(frm_buf_before), utils::size(frm_buf_after), 
            frm_buf_before, frm_buf_after, 
            before, after);
        /* Go through each element in the context before */
        for (int b = before - 1; b >= 0; b--) {
            /* Space */
            if (x >= 0) { frm_line[x--] = ' ';  }
            /* Draw phoneme */
            x -= drawPhon(frm_line, x, frm_buf_before[b]);
        }
        /* Fill with blanks */
        while(x >= 0) { frm_line[x--] = ' '; }
    }
    /* Draw the whole */
    display->setCursor(0, 3);
    display->write(frm_line, ui::Display::screenWidth());
}

/*******************************************
 * Logic
 *******************************************/

/* Render the screen */
void ui::screen::KeymapScreen::render(void* state, SerialLCD* display) {
    /* Get self */
    auto self = (KeymapScreen*)state;

    /* Draw the mode & note */
    IF_DIRTY(self->m_isDirty.mode, drawMode(display, synth::Engine::key().mode));
    IF_DIRTY(self->m_isDirty.note, drawNote(display, synth::Engine::note()));
    /* Draw the parameter */
    IF_DIRTY(self->m_isDirty.paramName, drawParamName(display, synth::Engine::key().mode, self->m_parameter.current));
    IF_DIRTY(self->m_isDirty.paramValue, drawParamValue(display, synth::Engine::key().mode, self->m_parameter.current, self->m_paramTarget.current));
    /* Draw the frame */
    bool smthFrameDirty = !!self->m_isDirty.frame || !!self->m_isDirty.frameDatamode;
    IF_DIRTY(smthFrameDirty, drawFrame(display, !!self->m_isDirty.frame, map_datamode(self->m_datamode.current), synth::Engine::workbenchIterator()));
    IF_DIRTY(self->m_isDirty.frameData, drawData(display, synth::Engine::workbenchIterator()));

    /* Clean dirty inputs */
    self->m_isDirty.value = 0x00;
}

void ui::screen::KeymapScreen::update(void* state, bool* dirty) {

    /* Wait till the screen is drawn, to avoid a weird race condition */
    Display::dry();

    /* Get self */
    auto self = (KeymapScreen*)state;

    /** EXTERNAL PARAMETERS **/
    /* Update note ; if it changed, dirty all relevant inputs */
    if (utils::preserved_changes_with(self->m_note, synth::Engine::note())) {
        /* Dirty mode, note, and parameter value */
        self->m_isDirty.mode = 1;
        self->m_isDirty.note = 1; 
        self->m_isDirty.paramValue = 1;
        self->m_isDirty.frame = 1;
        self->m_isDirty.frameDatamode = 1;
        self->m_isDirty.frameData = 1;
    }
    /* Update keymode ; if it changed, dirty it */
    if (utils::preserved_changes_with(self->m_keymode, synth::Engine::key().mode)) {
        /* Dirty keymode (and parameter name since it can change depending on mode ) */
        self->m_isDirty.mode = 1;
        self->m_isDirty.paramName = 1;
        self->m_isDirty.paramValue = 1;
        self->m_isDirty.frame = 1;
        self->m_isDirty.frameData = 1;
        self->m_isDirty.frameDatamode = 1;
    }
    /* If iterator changed */
    if (utils::preserved_changes_with(self->m_workbench, synth::Engine::workbenchIterator()) || self->m_workbenchValueChanged) {
        /* Clean */
        self->m_workbenchValueChanged = false;
        /* Dirty */
        self->m_isDirty.frame = 1;
        self->m_isDirty.frameData = 1;
        self->m_isDirty.frameDatamode = 1;
    }
    /* If datamode changed, dirty it */
    if (utils::preserved_changes_with(self->m_datamode, synth::tts_code_type(self->m_workbench.current.get()))) {
        /* Dirty datamode */
        self->m_isDirty.frameDatamode = 1;
    }
    
    /* If param target changed */
    if (utils::preserved_has_changed(self->m_paramTarget)) {
        /* Dirty */
        self->m_isDirty.paramValue = 1;
        /* Iterate */
        utils::preserved_iterate(self->m_paramTarget);
    }
    
    /* If param value changed */
    if (self->m_paramTarget.current && utils::preserved_changes_with(self->m_paramValue, *self->m_paramTarget.current)) {
        /* Dirty value */
        self->m_isDirty.paramValue = 1;
    }

    /** INTERNAL PARAMETERS **/
    /* If parameter has changed, dirty it */
    if (utils::preserved_has_changed(self->m_parameter)) {
        /* Dirty name and value */
        self->m_isDirty.paramName = 1;
        self->m_isDirty.paramValue = 1;
        /* Iterate */
        utils::preserved_iterate(self->m_parameter);
    }

    /* Check if need to flag dirty */
    *dirty = self->m_isDirty.value != 0;
}

/* Handle double click on next */
void ui::screen::KeymapScreen::onAltNextClick(bool isDoubleClick) {
    if (isDoubleClick) {
        /* Insert a value */
        synth::Engine::workbenchIterator().insert();
        /* Advance */
        synth::Engine::workbenchIterator().next();
        /* Say it's been modified */
        this->m_workbenchValueChanged = true;
    } else {
        /* Go to end of sequence */
        synth::Engine::workbenchIterator().end();
    }
}
/* Handle double click on prev */
void ui::screen::KeymapScreen::onAltPrevClick(bool isDoubleClick) {
    if (isDoubleClick) {
        /* Try go back */
        if (synth::Engine::workbenchIterator().previous()) {
            /* Remove current spot */
            synth::Engine::workbenchIterator().erase();
            /* Say it's been modified */
            this->m_workbenchValueChanged = true; 
        }
    } else {
        /* Go to beginning of sequence */
        synth::Engine::workbenchIterator().first(); 
    }
}

void ui::screen::KeymapScreen::input(void* state, const io::inputstate_t& inputs, io::outputstate_t& outputs) {
    /* Get self */
    auto self = (KeymapScreen*)state;

    /**** OUTPUT ****/
    /* Display cmd/phon mode */
    outputs.cmdphon = map_datamode(self->m_datamode.current) == DATAMODE_CMD;
    /* Check if alt has been pressed */
    if (!inputs.alt) {
        /* Alt is not pressed */
        outputs.individual = synth::Engine::key().mode == synth::KEY_MODE_LOCAL;
    } else {
        /* Alt is pressed */
        outputs.individual = 0;
    }

    /**** INPUT ****/

    /** Gutter part **/
    /* Menu encoder click */
    if (inputs.buttons.selector) {
        /* Go to settings screen */
        ui::Display::go<SettingScreen>();
        
    }

    /** Global part **/
    

    /** File part **/
    /* Load button */
    if (inputs.buttons.load) {
        /* Go to spell screen */
        if (inputs.alt) {
            ui::Display::go<SpellScreen>();
        }
    }

    /** Sound part **/
    /* Individual switch */
    if (inputs.buttons.individual) {
        /* Check alt */
        if (!inputs.alt) {
            /* Compute toggled mode */
            auto mode =
                synth::Engine::key().mode == synth::KEY_MODE_GLOBAL
                    ? synth::KEY_MODE_LOCAL
                    : synth::KEY_MODE_GLOBAL;
            /* Tidy workbench */
            synth::Engine::tidy();
            /* Change the mode */
            synth::Engine::key().mode = mode;
            /* Fetch to workbench */
            synth::Engine::fetch();
            
        }
    }

    /* Prelisten */
    if (inputs.buttons.prelisten) {
        /* Check alt */
        if (inputs.alt) {
            /* TODO : implement EXPLAIN command */
            audio::Soundcard::shutUp();
        } else {
            /* Copy workbench into special bench */
            synth::Lexicon::copy(synth::Engine::workbenchIterator().word(), self->m_prelistenBench);
            /* Set frequency */
            audio::Soundcard::frequency(synth::Engine::frequency());
            /* Set word and play */
            audio::Soundcard::word(synth::Lexicon::iterator(self->m_prelistenBench));
            audio::Soundcard::play();
        }
    }

    /* CMD/PHN swith */
    if (inputs.buttons.cmdphon) {
        /* Check alt */
        if (!inputs.alt) {
            /* Transform current value */
            synth::tts_code_transform(synth::Engine::workbenchIterator().at());
            /* Say it's been modified */
            self->m_workbenchValueChanged = true;
            /* Redraw parameter value */
        }
    }

    /* Next & previous buttons */
    if (inputs.buttons.next || inputs.buttons.prev) {
        /* Check if alt */
        if (!inputs.alt) {
            /* Go next */
            if (inputs.buttons.next) { synth::Engine::workbenchIterator().next(); }
            /* Go previous */
            else { synth::Engine::workbenchIterator().previous(); }
        } else {
            /* Go next */
            if (inputs.buttons.next) {
                /* Register click */
                self->m_gestures.altNext.rawClick();
            }
            /* Go previous */
            else { 
                /* Register click */
                self->m_gestures.altPrev.rawClick();
            }
        }
    }

    /* Pitch encoder */
    if (inputs.encoders.pitch) {
        /* Select the parameter */
        utils::preserved_update(self->m_parameter, !inputs.alt ? PARAM_PITCH : PARAM_SPEED);
        /* Select the parameter */
        switch (self->m_parameter.current) {
            case PARAM_PITCH: 
                utils::preserved_update(self->m_paramTarget, &synth::Engine::key().pitch); 
                break;
            case PARAM_SPEED: 
                utils::preserved_update(self->m_paramTarget, (uint8_t*)nullptr); 
                // utils::preserved_update(self->m_paramTarget, &synth::Engine::key().speed);   
                break;
        }
        /* Add to parameter */
        if (self->m_paramTarget.current) {
            *self->m_paramTarget.current += inputs.encoders.pitch;
        }
    }

    /* Data encoder */
    if (inputs.encoders.data) {
        /* Check if alt pressed */
        if (!inputs.alt) {
            /* Alt not pressed */ 
            /* Modify frame (TODO : do)*/
            synth::tts_code_delta(synth::Engine::workbenchIterator().at(), inputs.encoders.data);
            /* Say it's been modified */
            self->m_workbenchValueChanged = true;
        }
    }
}