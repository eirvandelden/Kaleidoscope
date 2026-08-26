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

EventHandlerResult AvailabilityLight::onFocusEvent(const char *command) {
  const char *cmd = PSTR("availability.color");

  if (::Focus.inputMatchesHelp(command))
    return ::Focus.printHelp(cmd);

  if (strcmp_P(command, cmd) != 0) return EventHandlerResult::OK;

  ::Focus.read(wanted_);

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
