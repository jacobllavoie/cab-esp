// flashing_amber_white_sequence.cpp

#include "config.h"
#include "globals.h"

void flashingAmberWhiteSequence(int brightness) {
  static unsigned long previousMillis = 0;
  static int sequenceStep = 0;
  static bool ledState = false;
  long interval = animationSpeed;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    ledState = !ledState;

    if (ledState) {
      byte rAmber = (255 * brightness) / 255;
      byte gAmber = (191 * brightness) / 255;
      byte bAmber = 0;

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
              leds.setPixelColor(i, rAmber, gAmber, bAmber);
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
              leds.setPixelColor(i, rAmber, gAmber, bAmber);
            }
          }
          break;
      }
    } else {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds.setPixelColor(i, 0, 0, 0);
      }
    }

    leds.show();
    if (ledState) {
      sequenceStep++;
      if (sequenceStep > 5) {
        sequenceStep = 0;
      }
    }
  }
}