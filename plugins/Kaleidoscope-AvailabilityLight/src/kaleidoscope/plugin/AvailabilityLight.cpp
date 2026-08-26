/* Kaleidoscope-AvailabilityLight -- Colours the whole keyboard on request
 * Copyright 2026 Etienne van Delden de la Haije
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "kaleidoscope/plugin/AvailabilityLight.h"

#include <Arduino.h>                   // for PSTR, strcmp_P, F
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial

#include "kaleidoscope/plugin/LEDControl.h"  // for LEDControl

namespace kaleidoscope {
namespace plugin {

namespace {

/* Every key lit at once draws far more current than a keyboard may take from a
 * USB port, so a colour is scaled down until the whole board fits the budget.
 * Only the brightness gives way; the ratio between the three channels — the
 * colour that was actually asked for — survives.
 *
 * This is the same worry that makes the stock firmware boot with its LEDs off,
 * to avoid over-taxing devices with little power to spare. A desktop machine
 * has more room than this allows, so a colour that comes out looking too dim is
 * a reason to raise the budget rather than to distrust the host.
 */
constexpr uint16_t kBrightestTheWholeBoardMayBe = 255;

cRGB withinThePowerBudget(cRGB colour) {
  const uint16_t asked_for = colour.r + colour.g + colour.b;

  if (asked_for <= kBrightestTheWholeBoardMayBe) return colour;

  cRGB affordable;
  affordable.r = colour.r * kBrightestTheWholeBoardMayBe / asked_for;
  affordable.g = colour.g * kBrightestTheWholeBoardMayBe / asked_for;
  affordable.b = colour.b * kBrightestTheWholeBoardMayBe / asked_for;

  return affordable;
}

}  // namespace

EventHandlerResult AvailabilityLight::onFocusEvent(const char *command) {
  const char *cmd = PSTR("availability.color");

  if (::Focus.inputMatchesHelp(command))
    return ::Focus.printHelp(cmd);

  if (strcmp_P(command, cmd) != 0) return EventHandlerResult::OK;

  cRGB asked_for;
  ::Focus.read(asked_for);
  wanted_ = withinThePowerBudget(asked_for);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult AvailabilityLight::onNameQuery() {
  return ::Focus.sendName(F("AvailabilityLight"));
}

void AvailabilityLight::TransientLEDMode::update() {
  ::LEDControl.set_all_leds_to(light_->wanted_);
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::AvailabilityLight AvailabilityLight;
