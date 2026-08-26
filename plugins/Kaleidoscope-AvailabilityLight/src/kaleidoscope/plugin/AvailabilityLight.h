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

#pragma once

#include "kaleidoscope/device/device.h"          // for cRGB
#include "kaleidoscope/event_handler_result.h"   // for EventHandlerResult
#include "kaleidoscope/plugin.h"                 // for Plugin
#include "kaleidoscope/plugin/LEDMode.h"         // for LEDMode
#include "kaleidoscope/plugin/LEDModeInterface.h"  // for LEDModeInterface

namespace kaleidoscope {
namespace plugin {

/* An LED mode whose colour is chosen by the host rather than by the firmware.
 *
 * The host says `availability.color <red> <green> <blue>` over Focus and the
 * whole board takes that colour. Something outside the keyboard decides what
 * the colours mean; all this plugin does is show one.
 */
class AvailabilityLight : public Plugin,
                          public LEDModeInterface {
 public:
  AvailabilityLight() {
    led_mode_name_ = "AvailabilityLight";
  }

  EventHandlerResult onNameQuery();
  EventHandlerResult onFocusEvent(const char *command);

  class TransientLEDMode : public LEDMode {
   public:
    explicit TransientLEDMode(const AvailabilityLight *light)
      : light_(light) {}

   protected:
    void update() final;

   private:
    const AvailabilityLight *light_;
  };

 private:
  cRGB wanted_{0, 0, 0};
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::AvailabilityLight AvailabilityLight;
