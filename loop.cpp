// loop.cpp

#include "config.h"
#include "globals.h"
#include "bluetooth_handler.h"
#include "serial_handler.h"
#include "wifi_handler.h"

void loop() {
  // Heartbeat
  static unsigned long previousMillis = 0;
  const unsigned long interval = 500;
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(HEARTBEAT_LED_PIN, !digitalRead(HEARTBEAT_LED_PIN));
  }

  handleWebServer();

  // Handle commands from different sources
  handleBluetoothInput();
  handleSerialInput();

  // Run active animation if one is set
  if (currentEffect == "rainbow") {
    rainbow(brightness);
  } else if (currentEffect == "chasing_rainbow") {
    chasingRainbow(brightness);
  } else if (currentEffect == "flashing_amber_white_sequence") {
    flashingAmberWhiteSequence(brightness);
  }
}