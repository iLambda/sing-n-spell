#include <mbed.h>

#include "io/controller.h"
#include "ui/display.h"
#include "ui/screens/bootscreen.h"
#include "ui/screens/keymapscreen.h"
#include "ui/screens/spellscreen.h"
#include "synth/engine.h"
#include "utils/debugging.h"

/* The screens */
ui::screen::BootScreen bootScreen;
ui::screen::KeymapScreen keymapScreen;
ui::screen::SpellScreen spellScreen;

int main() {

  /* Start initialization */
  #ifndef BUILD_CONFIG_DEBUG
    dbg::setLEDs(0x01);
  #else
    dbg::setLEDs(0x00);
  #endif
  
  /* Initialize controller */
  io::Controller::run();
  /* Initialize engine */
  synth::Engine::run();

  /* Initialize display */
  ui::Display::add(bootScreen);
  ui::Display::add(keymapScreen);
  ui::Display::add(spellScreen);
  ui::Display::go(bootScreen);
  ui::Display::run();

  /* End initialization */
  #ifndef BUILD_CONFIG_DEBUG
    dbg::setLEDs(0x03);
  #endif

  while(1) {
  }
}