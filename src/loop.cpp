// loop.cpp

#include "config.h"
#include "globals.h"
#include "ble_handler.h"
#include "serial_handler.h"
#include "wifi_handler.h"

void loop() {
  // Debug: print once at start
  static bool firstRun = true;
  if (firstRun) {
    Serial.println("Loop started");
    firstRun = false;
  }
  
  // Heartbeat
  static unsigned long previousMillis = 0;
  const unsigned long interval = 500;
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    // Toggle the addressable heartbeat pixel
    heartbeatPixelOn = !heartbeatPixelOn;
    if (heartbeatPixelOn) {
      // Dim green
      heartbeatPixel.setPixelColor(0, heartbeatPixel.Color(0, 32, 0));
    } else {
      heartbeatPixel.setPixelColor(0, 0); // off
    }
    heartbeatPixel.show();
  }

  // handleWebServer();
  // BLE is handled via callbacks in ble_handler; keep Serial input handler.
  handleSerialInput();

  // Run active animation if one is set
  if (currentEffect == "rainbow") {
    rainbow(brightness);
  } else if (currentEffect == "chasing_rainbow") {
    chasingRainbow(brightness);
  } else if (currentEffect == "amber_white_strobe") {
    amberWhiteStrobe(brightness);
  } else if (currentEffect == "green_white_strobe") {
    greenWhiteStrobe(brightness);
  }
}