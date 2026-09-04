# Kaleidoscope-AvailabilityLight

An LED mode whose colour is chosen from outside the keyboard.

Most LED modes decide for themselves what to show. This one does not: it waits to
be told. Something on the host — a home automation system, a status script,
whatever you like — says what colour the keyboard should be, and the whole board
fades to it. What a colour *means* is never the keyboard's business.

The name comes from what it was built for: showing the room whether you are free
to interrupt.

## Using the plugin

```c++
#include "Kaleidoscope.h"
#include "Kaleidoscope-AvailabilityLight.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-LEDControl.h"

KALEIDOSCOPE_INIT_PLUGINS(Focus, LEDControl, AvailabilityLight);

void setup() {
  Kaleidoscope.setup();
  LEDControl.activate(&AvailabilityLight);
}
```

`Focus` is required: without it there is no way to say what colour to show, and
the board stays dark.

## Focus commands

### `availability.color <red> <green> <blue>`

Fades the whole keyboard to that colour over about two seconds. Each value runs
from 0 to 255. Black fades the board to dark.

A colour that arrives part way through a fade redirects it rather than queueing
behind it — the new fade starts from whatever the keys are showing at that
moment — so a host sliding a colour picker around never makes the board stutter.

The colour is held in memory only. Unplugging the keyboard forgets it, and
whatever sets the colour is expected to say it again when the keyboard comes
back.

## Dimmer than you asked for

Every key lit at once draws far more current than a keyboard may take from a USB
port, so a colour that would go over budget is scaled down until the whole board
fits. Only the brightness gives way: the ratio between the three channels
survives, so the colour you asked for is still the colour you get, just softer.
Full white arrives as a soft grey.

This is the same worry that makes the stock Model 100 firmware boot with its LEDs
off, to avoid over-taxing hosts with little power to spare. A desktop machine has
more room than the budget here allows, so a colour that looks too dim is a reason
to raise `kBrightestTheWholeBoardMayBe` rather than to distrust the host.

## Further reading

The `basic` test under `tests/plugins/AvailabilityLight` is the specification:
each test names one thing the light does.
