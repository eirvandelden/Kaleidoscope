// -*- mode: c++ -*-

#include "Kaleidoscope.h"
#include "Kaleidoscope-AvailabilityLight.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-LEDControl.h"

// clang-format off

KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX

   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
          ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX
  )
)

// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(Focus, LEDControl, AvailabilityLight);

void setup() {
  Kaleidoscope.setup();
  LEDControl.activate(&AvailabilityLight);
}

void loop() {
  Kaleidoscope.loop();
}
