// main.cpp
#include <Arduino.h>

#include "config.h"
#include "globals.h"
#include <EEPROM.h>
// Instantiate hardware objects here where config.h macros are visible
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRBW + NEO_KHZ800);

// State variables
String currentEffect = "";
byte currentColorRed = 0;
byte currentColorGreen = 0;
byte currentColorBlue = 0;
byte currentColorWhite = 0;

// Heartbeat pixel: a single WS2812/WS6812 LED on HEARTBEAT_PIN
Adafruit_NeoPixel heartbeatPixel = Adafruit_NeoPixel(HEARTBEAT_NUM_LEDS, HEARTBEAT_PIN, NEO_GRB + NEO_KHZ800);
bool heartbeatPixelOn = false;

// BluetoothSerial instance is defined in bluetooth_handler.cpp