// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define DEBUG_ENABLED 0 // Set to 1 to enable debug messages, 0 to disable

#if DEBUG_ENABLED
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINTF(x, ...) Serial.printf(x, ##__VA_ARGS__)
  #define DEBUG_PRINT_BT(x)   SerialBT.print(x)
  #define DEBUG_PRINTLN_BT(x) SerialBT.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x, ...)
  #define DEBUG_PRINT_BT(x)
  #define DEBUG_PRINTLN_BT(x)
#endif

#include "credentials.h"

// LED settings
#define NUM_LEDS 56
#define DATA_PIN 13

// Heartbeat LED settings
#define HEARTBEAT_LED_PIN 2

// EEPROM addresses
#define EEPROM_EFFECT_ADDR 0
#define EEPROM_BRIGHTNESS_ADDR 10
#define EEPROM_RED_ADDR 12
#define EEPROM_GREEN_ADDR 13
#define EEPROM_BLUE_ADDR 14
#define EEPROM_WHITE_ADDR 15
#define EEPROM_SPEED_ADDR 16
#define EEPROM_SIZE 512


// Color lookup table
struct ColorDefinition {
  const char* name;
  uint32_t color;
};

const ColorDefinition colorTable[] = {
  {"red",    0xFF000000},
  {"green",  0x00FF0000},
  {"blue",   0x0000FF00},
  {"white",  0x000000FF},
  {"purple", 0x80008000},
  {"yellow", 0xFFFF0000},
  {"cyan",   0x00FFFF00},
  {"black",  0x00000000},
};

#endif