// setup.cpp

#include "config.h"
#include "globals.h"
#include "ble_handler.h"
#include "wifi_handler.h"

void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Cab-ESP is ready!");

  // setup Wifi & OTA (optional)
  // setupWifi();
  // setupOTA();

  // Initialize BLE and services
  setupBLE();
  Serial.println("BLE init done");

  // Initialize heartbeat addressable pixel
  // Temporarily disabled - may be causing watchdog reset on GPIO8
  // heartbeatPixel.begin();
  // heartbeatPixel.setBrightness(HEARTBEAT_BRIGHTNESS);
  // heartbeatPixel.setPixelColor(0, 0);
  // heartbeatPixel.show();
  Serial.println("Heartbeat LED init skipped (disabled)");

  EEPROM.begin(EEPROM_SIZE);
  leds.begin();
  Serial.println("Main LEDs init done");
  
  readLedStateFromEEPROM();
  Serial.println("EEPROM read done");

  // Boot-up Animation
  Serial.println("Running boot animation...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 255, 0, 0); // Red
    if (i % 10 == 0) yield(); // Let system tasks run
  }
  leds.show();
  delay(500);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0, 255, 0); // Green
    if (i % 10 == 0) yield();
  }
  leds.show();
  delay(500);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0, 0, 255); // Blue
    if (i % 10 == 0) yield();
  }
  leds.show();
  delay(500);

  // Clear LEDs after animation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0, 0, 0);
    if (i % 10 == 0) yield();
  }
  leds.show();
  Serial.println("Boot animation done");

  // Apply saved state from EEPROM
  applyLedState();
  
  Serial.println("Setup complete!");
  delay(100); // Give time for serial to flush
}