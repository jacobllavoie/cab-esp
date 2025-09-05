// setup.cpp

#include "config.h"
#include "globals.h"
#include "bluetooth_handler.h"
#include "wifi_handler.h"

void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Cab-ESP is ready!");

  // setupWifi();
  setupBluetooth();

  pinMode(HEARTBEAT_LED_PIN, OUTPUT);
  digitalWrite(HEARTBEAT_LED_PIN, HIGH);

  EEPROM.begin(EEPROM_SIZE);
  leds.begin();
  readLedStateFromEEPROM();

  // Boot-up Animation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 255, 0, 0); // Red
  }
  leds.show();
  delay(500);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0, 255, 0); // Green
  }
  leds.show();
  delay(500);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0, 0, 255); // Blue
  }
  leds.show();
  delay(500);

  // Clear LEDs after animation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0, 0, 0);
  }
  leds.show();

  // Apply saved state from EEPROM
  applyLedState();
}