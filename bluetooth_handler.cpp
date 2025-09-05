// bluetooth_handler.cpp

#include "bluetooth_handler.h"
#include "globals.h" //add the globals include

BluetoothSerial SerialBT;

void setupBluetooth() {
  SerialBT.begin(BLUETOOTH_NAME);
  Serial.println("Bluetooth is ready!");
  SerialBT.println("Bluetooth is ready!"); // Send to Bluetooth as well
  displayHelp();
}

void handleBluetoothInput() {
  static String commandBT;
  static String commandSerial;
  char endMarker = '\n';

  // Process Bluetooth commands
  while (SerialBT.available() > 0) {
    char receivedChar = SerialBT.read();
    if (receivedChar == endMarker) {
      commandBT.trim();
      Serial.println("Received from Bluetooth: " + commandBT);
      processCommand(commandBT);
      commandBT = ""; // Clear for next command
    } else {
      commandBT += receivedChar;
    }
  }

  // Process Serial commands
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == endMarker) {
      commandSerial.trim();
      Serial.println("Received from Serial Monitor: " + commandSerial);
      processCommand(commandSerial);
      commandSerial = ""; // Clear for next command
    } else {
      commandSerial += receivedChar;
    }
  }
}

void processCommand(String command) {
  // Handle single-character commands first
  if (command.equalsIgnoreCase("H")) {
    displayHelp();
    return; // Exit after handling
  }
  if (command.equalsIgnoreCase("R")) {
    rebootESP();
    return; // Exit after handling
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
        Serial.println("Color not found");
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
        Serial.println("Invalid brightness value");
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
        Serial.println("Invalid speed value");
        SerialBT.println("Invalid speed value");
      }
    } else {
      Serial.println("Unknown Command type");
      SerialBT.println("Unknown Command type");
    }
  } else {
    Serial.println("Unknown command format.");
    SerialBT.println("Unknown command format.");
  }
}