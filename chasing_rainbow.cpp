// chasing_rainbow.cpp

#include "config.h"
#include "globals.h"

void chasingRainbow(int brightness) {
  static unsigned long previousMillis = 0;
  static int j = 0;

  if (millis() - previousMillis >= animationSpeed) {
    previousMillis = millis();

    for (int i = 0; i < NUM_LEDS; i++) {
      uint32_t color = WheelSmooth(((i * 256 / NUM_LEDS) + j) & 255);

      byte r = (color >> 16) & 0xFF;
      byte g = (color >> 8) & 0xFF;
      byte b = color & 0xFF;

      r = (r * brightness) / 255;
      g = (g * brightness) / 255;
      b = (b * brightness) / 255;

      leds.setPixelColor(i, r, g, b);
    }
    leds.show();
    j++;
  }
}