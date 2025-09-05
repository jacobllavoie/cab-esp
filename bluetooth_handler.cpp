// bluetooth_handler.cpp

#include "bluetooth_handler.h"
#include "globals.h"

BluetoothSerial SerialBT;

void setupBluetooth() {
  SerialBT.begin(BLUETOOTH_NAME);
  Serial.println("Bluetooth is ready!");
  SerialBT.println("Bluetooth is ready!");
  displayHelp();
}

// This function now ONLY checks for and processes Bluetooth data.
void handleBluetoothInput() {
  static String commandBT;
  char endMarker = '\n';

  while (SerialBT.available() > 0) {
    char receivedChar = SerialBT.read();
    // Check for either character
    if (receivedChar == '\n' || receivedChar == '\r') {
      commandBT.trim();
      Serial.println("Received from Bluetooth: " + commandBT);
      processBluetoothCommand(commandBT);
      commandBT = ""; // Clear for next command
    } else {
      commandBT += receivedChar;
    }
  }
}

// Renamed from processCommand for clarity
void processBluetoothCommand(String command) {
  // This function's logic is the same as your processCommand, but the name is clearer.
  // I am including the full corrected logic from our previous discussions.
  
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

  // Handle commands with a comma (e.g., "C,red")
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
          SerialBT.println("Color set to: " + commandData);
          break;
        }
      }
      if (!colorFound) {
        SerialBT.println("Color not found");
      }
    } else if (commandType.equalsIgnoreCase("B")) { // Set Brightness
      int newBrightness = commandData.toInt();
      if (newBrightness >= 0 && newBrightness <= 255) {
        brightness = newBrightness;
        saveLedStateToEEPROM();
        applyLedState();
        SerialBT.println("Brightness set to: " + String(brightness));
      } else {
        SerialBT.println("Invalid brightness value");
      }
    } else if (commandType.equalsIgnoreCase("E")) { // Set Effect
      currentEffect = commandData;
      saveLedStateToEEPROM();
      applyLedState();
      SerialBT.println("Effect set to: " + currentEffect);
    } else if (commandType.equalsIgnoreCase("S")) { // Set Speed
      int newSpeed = commandData.toInt();
      if (newSpeed > 0) {
        animationSpeed = newSpeed;
        saveLedStateToEEPROM();
        applyLedState();
        SerialBT.println("Speed set to: " + String(animationSpeed));
      } else {
        SerialBT.println("Invalid speed value");
      }
    } else {
      SerialBT.println("Unknown Command type");
    }
  } else {
    SerialBT.println("Unknown command format.");
  }
}