#ifndef __SING_N_SPELL_UI_SCREEN_KEYMAPSCREEN
#define __SING_N_SPELL_UI_SCREEN_KEYMAPSCREEN

#include <mbed.h>
#include <serlcd.h>

#include "io/state.h"
#include "synth/keymap.h"
#include "ui/screen.h"
#include "utils/data/preserved.h"

#define KEYMAPSCREEN_OUTPUT_STATE_NOMOD     0
#define KEYMAPSCREEN_OUTPUT_STATE_ALT       1
#define KEYMAPSCREEN_OUTPUT_STATES          2

namespace ui {
    namespace screen {

        class KeymapScreen {
            
            public:
                /* The current displayed parameter */
                enum Parameter {
                    PARAM_PITCH = 0,
                    PARAM_SPEED = 1
                };
                /* The data mode */
                enum DataMode {
                    DATAMODE_PHN = 0,
                    DATAMODE_CMD = 1
                };

            private:
                /* The current parameter */
                utils::preserved_t<Parameter> m_parameter;
                /* The selected note */
                utils::preserved_t<uint8_t> m_note;
                /* The key mode mode */
                utils::preserved_t<synth::keymode_t> m_keymode;
                /* The cmd/phon mode */
                utils::preserved_t<DataMode> m_datamode;
                /* Did the iterator change ? */
                utils::preserved_t<synth::worditerator_t> m_workbench;
                /* The current parameter */
                utils::preserved_t<uint8_t*> m_paramTarget;
                /* The current parameter value */
                utils::preserved_t<uint8_t> m_paramValue;
                
                /* The dirty flags */
                union dirtyflags_t {
                    /* The value as an integer */
                    uint32_t value;
                    /* The flags */
                    struct {
                        uint8_t mode : 1;
                        uint8_t note : 1;
                        uint8_t paramName : 1;
                        uint8_t paramValue : 1;
                        uint8_t frame : 1;
                        uint8_t frameDatamode : 1;
                        uint8_t frameData : 1;
                    };
                } m_isDirty;
                /* The accumulated input state */
                // io::inputstate_t m_inputs;

            public:
                KeymapScreen();                

            public:
                /* Return a screen_t representing this screen */
                screen_t get() { return { (void*)this, &reset, &render, &update, &input }; }
                /* Return the ID of this screen */
                static constexpr uint8_t getID() { return 1; }

            private:
                /* Enter the screen */
                static void reset(void* state);
                /* Render the screen */
                static void render(void* state, SerialLCD* display);
                /* Update the screen */
                static void update(void* state, bool* dirty);
                /* Parse inputs */
                static void input(void* state, const io::inputstate_t& inputs, io::outputstate_t& outputs);
        };
    }

};

#endif