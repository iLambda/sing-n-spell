#ifndef __SING_N_SPELL_UI_SCREEN_SETTINGSCREEN
#define __SING_N_SPELL_UI_SCREEN_SETTINGSCREEN

#include <mbed.h>
#include <serlcd.h>

#include "ui/screen.h"

namespace ui {
    namespace screen {

        class SettingScreen {

            public:
                /* The settings */
                struct settings_t {
                    /* The screen color */
                    uint8_t background;
                    /* The contrast */
                    uint8_t contrast;
                };

            private:
                /* A setting */
                struct setting_t {
                    const char* text;
                    uint8_t* target;
                };

            private:
                /* The settings */
                static settings_t m_settings;
                /* The setting list */
                static const setting_t m_settingList[];
                /* The dirty flags */
                union dirtyflags_t {
                    /* The value as an integer */
                    uint32_t value;
                    /* The flags */
                    struct {
                        uint8_t scrollbarEnd : 1;
                        uint8_t scrollbarMiddle : 1;
                    };
                } m_isDirty;
                /* The current selected item */
                uint8_t m_selected;

            public:
                SettingScreen() {}            

            public:
                /* Return a screen_t representing this screen */
                screen_t get() { return { (void*)this, &reset, &render, &update, &input }; }
                /* Return the ID of this screen */
                static constexpr uint8_t getID() { return 3; }

                /* Return the settings */
                MBED_FORCEINLINE static const settings_t& settings() { return m_settings; }

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