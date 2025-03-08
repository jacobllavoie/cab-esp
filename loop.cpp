// loop.cpp

#include "config.h"
#include "globals.h"

void loop() {
  // Heartbeat
  static unsigned long previousMillis = millis();
  const unsigned long interval = 1000;
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(HEARTBEAT_LED_PIN, !digitalRead(HEARTBEAT_LED_PIN));
  }

  // Run active animation
  if (currentEffect == "rainbow") {
    rainbow(brightness);
  } else if (currentEffect == "chasing_rainbow") {
    chasingRainbow(brightness);
  } else if (currentEffect == "flashing_amber_white_sequence") {
    flashingAmberWhiteSequence(brightness);
  }

  // Set OVERALL BRIGHTNESS after the animations
  leds.setBrightness(map(brightness, 0, 100, 0, 255));
  leds.show();

  // Serial command processing
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processSerialCommand(command);
  }
}

void processSerialCommand(String command) {
  if (command.startsWith("C,")) {
    String colorName = command.substring(2);
    Serial.print("Color name received: ");
    Serial.println(colorName);

    bool colorFound = false;
    for (int i = 0; i < sizeof(colorTable) / sizeof(colorTable[0]); i++) {
      if (colorName.equalsIgnoreCase(colorTable[i].name)) {
        uint32_t color = colorTable[i].color;
        currentColorRed = (color >> 24) & 0xFF;
        currentColorGreen = (color >> 16) & 0xFF;
        currentColorBlue = (color >> 8) & 0xFF;
        currentColorWhite = color & 0xFF;
        applyLedState();
        saveLedStateToEEPROM(); // Save after color change
        Serial.print("Color set to: ");
        Serial.println(colorTable[i].name);
        colorFound = true;
        break;
      }
    }
    if (!colorFound) {
      Serial.println("Color not found");
    }
  } else if (command.startsWith("B,")) {
    String brightnessValue = command.substring(2);
    brightness = brightnessValue.toInt();
    brightness = constrain(brightness, 0, 255);
    Serial.print("Brightness set to: ");
    Serial.println(brightness);
    applyLedState();
    saveLedStateToEEPROM(); // Save after brightness change
  } else if (command.startsWith("E,")) {
    String effectName = command.substring(2);
    currentEffect = effectName;
    Serial.print("Effect set to: ");
    Serial.println(effectName);
    applyLedState();
    saveLedStateToEEPROM(); // Save after effect change
  } else if (command.startsWith("S,")) {
    String speedValue = command.substring(2);
    animationSpeed = speedValue.toInt();
    animationSpeed = constrain(animationSpeed, 1, 255);
    Serial.print("Animation speed set to: ");
    Serial.println(animationSpeed);
    applyLedState();
    saveLedStateToEEPROM(); // Save after speed change
  } else if (command.equalsIgnoreCase("H")) {
    displayHelp();
  } else {
    Serial.println("Unknown command");
  }
}