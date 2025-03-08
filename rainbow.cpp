// rainbow.cpp

#include "config.h"
#include "globals.h"

void rainbow(int brightness) {
  static byte hue = 0;

  uint32_t color = WheelSmooth(hue);

  byte r = (color >> 16) & 0xFF;
  byte g = (color >> 8) & 0xFF;
  byte b = color & 0xFF;

  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, r, g, b);
  }
  leds.show();

  hue++;
  delay(animationSpeed);
}