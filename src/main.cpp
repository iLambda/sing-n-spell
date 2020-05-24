#include <mbed.h>

#include "audio/soundcard.h"
#include "io/controller.h"
#include "ui/display.h"
#include "ui/screens/bootscreen.h"
#include "ui/screens/keymapscreen.h"
#include "ui/screens/spellscreen.h"
#include "ui/screens/settingscreen.h"
#include "synth/engine.h"
#include "utils/debugging.h"

/* The screens */
ui::screen::BootScreen bootScreen;
ui::screen::KeymapScreen keymapScreen;
ui::screen::SpellScreen spellScreen;
ui::screen::SettingScreen settingScreen;

int main() {

    /* Start initialization */
    #ifndef BUILD_CONFIG_DEBUG
        dbg::setLEDs(0x01);
    #else
        dbg::setLEDs(0x00);
    #endif
    
    /* Initialize soundcard */
    audio::Soundcard::run();
    /* Initialize controller */
    io::Controller::run();
    /* Initialize engine */
    synth::Engine::run();

    /* Initialize display */
    ui::Display::add(bootScreen);
    ui::Display::add(keymapScreen);
    ui::Display::add(spellScreen);
    ui::Display::add(settingScreen);
    ui::Display::go(bootScreen);
    ui::Display::run();

    /* End initialization */
    #ifndef BUILD_CONFIG_DEBUG
        dbg::setLEDs(0x03);
    #endif

    /* Loop forever */
    while(1) { }
}