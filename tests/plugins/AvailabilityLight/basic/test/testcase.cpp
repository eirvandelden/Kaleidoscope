#include "testing/setup-googletest.h"

#include "Kaleidoscope-LEDControl.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

class AvailabilityLightTest : public VirtualDeviceTest {
 protected:
  // LEDControl only pushes colours to the board every so often, so asking for a
  // colour and reading the keys back in the same cycle would always read stale.
  static constexpr size_t kMillisToReachTheKeys = 100;

  void AskForColour(const char *command) {
    sim_.SendFocusCommand(command);
    sim_.RunForMillis(kMillisToReachTheKeys);
  }

  void ExpectTheWholeBoardShows(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t key = 0; key < Runtime.device().led_count; ++key) {
      cRGB shown = ::LEDControl.getCrgbAt(key);

      ASSERT_EQ(shown.r, red) << "key " << int(key) << " is showing the wrong amount of red";
      ASSERT_EQ(shown.g, green) << "key " << int(key) << " is showing the wrong amount of green";
      ASSERT_EQ(shown.b, blue) << "key " << int(key) << " is showing the wrong amount of blue";
    }
  }
};

TEST_F(AvailabilityLightTest, AskingForRedTurnsEveryKeyRed) {
  RunCycle();

  AskForColour("availability.color 200 0 0");

  ExpectTheWholeBoardShows(200, 0, 0);
}

TEST_F(AvailabilityLightTest, FullWhiteIsDimmedToWhatTheKeyboardCanPower) {
  RunCycle();

  AskForColour("availability.color 255 255 255");

  ExpectTheWholeBoardShows(85, 85, 85);
}

TEST_F(AvailabilityLightTest, ADimmedColourIsStillTheColourThatWasAskedFor) {
  RunCycle();

  AskForColour("availability.color 255 128 0");

  ExpectTheWholeBoardShows(169, 85, 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
