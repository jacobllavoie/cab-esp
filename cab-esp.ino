// cab-esp.ino

#include "config.h"
#include "globals.h"
#include <EEPROM.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
String currentEffect = "";
byte currentColorRed = 0;
byte currentColorGreen = 0;
byte currentColorBlue = 0;
byte currentColorWhite = 0;