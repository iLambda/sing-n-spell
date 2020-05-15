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
  dbg::setLEDs(0x01);
  
  /* Initialize controller */
  io::Controller::run();

  /* Initialize engine */
  synth::Engine::run();
  //synth::Engine::workbenchIterator().next();

  /* Initialize display */
  ui::Display::add(bootScreen.getID(), bootScreen.get());
  ui::Display::add(keymapScreen.getID(), keymapScreen.get());
  ui::Display::add(spellScreen.getID(), spellScreen.get());
  ui::Display::go(bootScreen.getID());
  ui::Display::run();

  /* End initialization */
  dbg::setLEDs(0x03);

  while(1) {
  }
}