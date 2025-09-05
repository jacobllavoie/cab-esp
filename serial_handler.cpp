// serial_handler.cpp

#include "serial_handler.h"
#include "globals.h"
#include "config.h"

// This function now uses a non-blocking method to read serial data
void handleSerialInput() {
  static String commandSerial;
  char endMarker = '\n';

  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    // Check for either character
    if (receivedChar == '\n' || receivedChar == '\r') {
      commandSerial.trim();
      Serial.println("Received from Serial Monitor: " + commandSerial);
      processSerialCommand(commandSerial);
      commandSerial = ""; // Clear for next command
    } else {
      commandSerial += receivedChar;
    }
  }
}

// This function processes the command received from the serial monitor
void processSerialCommand(String command) {
  // Handle single-character commands first
  if (command.equalsIgnoreCase("H")) {
    displayHelp();
    return;
  }
  if (command.equalsIgnoreCase("R")) {
    rebootESP();
    return;
  }
  if (command.equalsIgnoreCase("STATUS")) { // <-- Add this block
    displayStatus();
    return;
  }

  // Handle commands with a comma
  int commaIndex = command.indexOf(',');
  if (commaIndex > 0) {
    String commandType = command.substring(0, commaIndex);
    String commandData = command.substring(commaIndex + 1);

    if (commandType.equalsIgnoreCase("C")) { // Set Color
      bool colorFound = false;
      for (int i = 0; i < sizeof(colorTable) / sizeof(colorTable[0]); i++) {
        if (commandData.equalsIgnoreCase(colorTable[i].name)) {
          uint32_t color = colorTable[i].color;
          currentColorRed = (color >> 24) & 0xFF;
          currentColorGreen = (color >> 16) & 0xFF;
          currentColorBlue = (color >> 8) & 0xFF;
          currentColorWhite = color & 0xFF;
          currentEffect = "";
          saveLedStateToEEPROM();
          applyLedState();
          colorFound = true;
          Serial.println("Color set to: " + String(colorTable[i].name));
          break;
        }
      }
      if (!colorFound) {
        Serial.println("Color not found");
      }
    } else if (commandType.equalsIgnoreCase("B")) { // Set Brightness
      brightness = commandData.toInt();
      brightness = constrain(brightness, 0, 255);
      saveLedStateToEEPROM();
      applyLedState();
      Serial.println("Brightness set to: " + String(brightness));
    } else if (commandType.equalsIgnoreCase("E")) { // Set Effect
      currentEffect = commandData;
      saveLedStateToEEPROM();
      applyLedState();
      Serial.println("Effect set to: " + currentEffect);
    } else if (commandType.equalsIgnoreCase("S")) { // Set Speed
      animationSpeed = commandData.toInt();
      animationSpeed = constrain(animationSpeed, 1, 255);
      saveLedStateToEEPROM();
      applyLedState();
      Serial.println("Speed set to: " + String(animationSpeed));
    } else {
      Serial.println("Unknown Command type");
    }
  } else {
    Serial.println("Unknown command format.");
  }
}