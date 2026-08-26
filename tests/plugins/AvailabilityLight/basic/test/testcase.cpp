#include "testing/setup-googletest.h"

#include "Kaleidoscope-LEDControl.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

class AvailabilityLightTest : public VirtualDeviceTest {
 protected:
  // Comfortably past the couple of seconds a fade takes, so a board that has
  // stopped moving really has arrived rather than merely paused.
  static constexpr size_t kMillisForTheBoardToArrive = 3000;

  void AskForColour(const char *command) {
    sim_.SendFocusCommand(command);
  }

  void WaitUntilTheBoardHasArrived() {
    sim_.RunForMillis(kMillisForTheBoardToArrive);
  }

  cRGB ShownOnTheFirstKey() {
    return ::LEDControl.getCrgbAt(0);
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
  WaitUntilTheBoardHasArrived();

  ExpectTheWholeBoardShows(200, 0, 0);
}

TEST_F(AvailabilityLightTest, FullWhiteIsDimmedToWhatTheKeyboardCanPower) {
  RunCycle();

  AskForColour("availability.color 255 255 255");
  WaitUntilTheBoardHasArrived();

  ExpectTheWholeBoardShows(85, 85, 85);
}

TEST_F(AvailabilityLightTest, ADimmedColourIsStillTheColourThatWasAskedFor) {
  RunCycle();

  AskForColour("availability.color 255 128 0");
  WaitUntilTheBoardHasArrived();

  ExpectTheWholeBoardShows(169, 85, 0);
}

TEST_F(AvailabilityLightTest, TheBoardFadesToANewColourRatherThanJumpingToIt) {
  RunCycle();
  AskForColour("availability.color 240 0 0");
  WaitUntilTheBoardHasArrived();

  AskForColour("availability.color 0 0 240");
  sim_.RunForMillis(1000);

  cRGB midway = ShownOnTheFirstKey();
  EXPECT_LT(midway.r, 240) << "the colour it was showing has not begun to fade out";
  EXPECT_GT(midway.r, 0) << "the colour it was showing vanished at once instead of fading out";
  EXPECT_GT(midway.b, 0) << "the colour that was asked for has not begun to fade in";
  EXPECT_LT(midway.b, 240) << "the colour that was asked for arrived at once instead of fading in";
}

TEST_F(AvailabilityLightTest, AColourAskedForMidFadeRedirectsItRatherThanWaitingItsTurn) {
  RunCycle();
  AskForColour("availability.color 240 0 0");
  WaitUntilTheBoardHasArrived();

  AskForColour("availability.color 0 0 240");
  sim_.RunForMillis(1000);
  AskForColour("availability.color 240 0 0");
  WaitUntilTheBoardHasArrived();

  ExpectTheWholeBoardShows(240, 0, 0);
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
