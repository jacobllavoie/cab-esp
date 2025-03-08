// utility_functions.cpp

#include "config.h"
#include "globals.h"

void displayHelp() {
  Serial.println("**Help Menu**");
  Serial.println("C,<color_name>: Set color (e.g., C,red)");
  Serial.println("B,<brightness>: Set brightness (0-255) (e.g., B,128)");
  Serial.println("E,<effect_name>: Set effect (e.g., E,rainbow)");
  Serial.println("S,<speed>: Set speed (ms) (e.g., S,20)");
  Serial.println("H: Display help");
  Serial.println("Available Effects:");
  for (int i = 0; i < numberOfEffects; i++) {
    Serial.print("  "); Serial.println(effects[i].name);
  }
}

uint32_t WheelSmooth(byte pos) {
  float r, g, b;

  if (pos < 85) {
    r = 255 - pos * 3;
    g = pos * 3;
    b = 0;
  } else if (pos < 170) {
    pos -= 85;
    r = 0;
    g = 255 - pos * 3;
    b = pos * 3;
  } else {
    pos -= 170;
    r = pos * 3;
    g = 0;
    b = 255 - pos * 3;
  }

  // Use a sine wave for smoother transitions
  r = 127.5 + 127.5 * sin(PI * (r / 255.0 - 0.5));
  g = 127.5 + 127.5 * sin(PI * (g / 255.0 - 0.5));
  b = 127.5 + 127.5 * sin(PI * (b / 255.0 - 0.5));

  return (uint32_t)((byte)r << 16 | (byte)g << 8 | (byte)b);
}