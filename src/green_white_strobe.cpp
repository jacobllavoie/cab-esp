// green_white_strobe.cpp

#include "config.h"
#include "globals.h"

void greenWhiteStrobe(int brightness) {
  static unsigned long previousMillis = 0;
  static int sequenceStep = 0;
  static bool ledState = false;
  long interval = animationSpeed;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    ledState = !ledState;

    if (ledState) {
      byte rGreen = 0;
      byte gGreen = (255 * brightness) / 255;
      byte bGreen = 0;

      byte rWhite = 255;
      byte gWhite = 255;
      byte bWhite = 255;

      int half = (NUM_LEDS + 1) / 2;

      switch (sequenceStep) {
        case 0:
        case 1:
        case 2:
          for (int i = 0; i < NUM_LEDS; i++) {
            if (i == (NUM_LEDS / 2) && (NUM_LEDS % 2 != 0)) {
              leds.setPixelColor(i, 0, 0, 0);
            } else if (i < half) {
              leds.setPixelColor(i, rGreen, gGreen, bGreen);
            } else {
              leds.setPixelColor(i, rWhite, gWhite, bWhite);
            }
          }
          break;

        case 3:
        case 4:
        case 5:
          for (int i = 0; i < NUM_LEDS; i++) {
            if (i == (NUM_LEDS / 2) && (NUM_LEDS % 2 != 0)) {
              leds.setPixelColor(i, 0, 0, 0);
            } else if (i < half) {
              leds.setPixelColor(i, rWhite, gWhite, bWhite);
            } else {
              leds.setPixelColor(i, rGreen, gGreen, bGreen);
            }
          }
          break;
      }

      leds.show();

      sequenceStep++;
      if (sequenceStep > 5) {
        sequenceStep = 0;
      }
    } else {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds.setPixelColor(i, 0, 0, 0);
      }
      leds.show();
    }
  }
}
