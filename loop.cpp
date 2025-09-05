// loop.cpp

#include "config.h"
#include "globals.h"
#include "bluetooth_handler.h"

void loop() {
  // Heartbeat
  static unsigned long previousMillis = millis();
  const unsigned long interval = 1000;
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(HEARTBEAT_LED_PIN, !digitalRead(HEARTBEAT_LED_PIN));
  }

  // This one function now handles all commands from BT and Serial
  handleCommandInput();

  // Run active animation if one is set
  if (currentEffect == "rainbow") {
    rainbow(brightness);
  } else if (currentEffect == "chasing_rainbow") {
    chasingRainbow(brightness);
  } else if (currentEffect == "flashing_amber_white_sequence") {
    flashingAmberWhiteSequence(brightness);
  }
}