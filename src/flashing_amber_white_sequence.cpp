// flashing_amber_white_sequence.cpp

#include "config.h"
#include "globals.h"

void amberWhiteStrobe(int brightness) {
  static unsigned long previousMillis = 0;
  static int sequenceStep = 0;
  static bool ledState = false;
  
  // Calculate on and off intervals based on duty cycle
  long onInterval = (animationSpeed * dutyCycle) / 100;
  long offInterval = (animationSpeed * (100 - dutyCycle)) / 100;
  long interval = ledState ? onInterval : offInterval;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    ledState = !ledState;

    if (ledState) {
      byte rAmber = (255 * brightness) / 255;
      byte gAmber = (191 * brightness) / 255;
      byte bAmber = 0;
      byte wAmber = 0;

      byte rWhite = 0;
      byte gWhite = 0;
      byte bWhite = 0;
      byte wWhite = (255 * brightness) / 255;

      int half = (NUM_LEDS + 1) / 2;

      switch (sequenceStep) {
        case 0:
        case 1:
        case 2:
          for (int i = 0; i < NUM_LEDS; i++) {
            if (i == (NUM_LEDS / 2) && (NUM_LEDS % 2 != 0)) {
              leds.setPixelColor(i, 0, 0, 0, 0);
            } else if (i < half) {
              leds.setPixelColor(i, rAmber, gAmber, bAmber, wAmber);
            } else {
              leds.setPixelColor(i, rWhite, gWhite, bWhite, wWhite);
            }
          }
          break;

        case 3:
        case 4:
        case 5:
          for (int i = 0; i < NUM_LEDS; i++) {
            if (i == (NUM_LEDS / 2) && (NUM_LEDS % 2 != 0)) {
              leds.setPixelColor(i, 0, 0, 0, 0);
            } else if (i < half) {
              leds.setPixelColor(i, rWhite, gWhite, bWhite, wWhite);
            } else {
              leds.setPixelColor(i, rAmber, gAmber, bAmber, wAmber);
            }
          }
          break;
      }
    } else {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds.setPixelColor(i, 0, 0, 0, 0);
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