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

void handleCommandInput() {
  if (SerialBT.available() > 0 || Serial.available() > 0) {
    String command;
    if (SerialBT.available() > 0) {
      command = SerialBT.readStringUntil('\n');
      Serial.println("Received from Bluetooth: " + command);
    } else {
      command = Serial.readStringUntil('\n');
      Serial.println("Received from Serial Monitor: " + command);
    }
    command.trim();
    processCommand(command);
  }
}

void processCommand(String command) {
  int commaIndex = command.indexOf(',');
  if (commaIndex > 0 || command == "H") {
    String commandType;
    String commandData;
    if (command == "H") {
      displayHelp();
    } else {
      commandType = command.substring(0, commaIndex);
      commandData = command.substring(commaIndex + 1);

      if (commandType == "C") { // Set Color
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
            colorFound = true;
            SerialBT.println("Color set to: " + commandData); // Feedback
            break;
          }
        }
        if (!colorFound) {
          Serial.println("Color not found");
          SerialBT.println("Color not found"); // Feedback
        }
      } else if (commandType == "B") { // Set Brightness
        int newBrightness = commandData.toInt(); //get new brightness.
        if (newBrightness >= 0 && newBrightness <= 255){ //check for valid range.
          brightness = newBrightness;
          saveLedStateToEEPROM();
          SerialBT.println("Brightness set to: " + String(brightness)); // Feedback
        } else {
          Serial.println("Invalid brightness value");
          SerialBT.println("Invalid brightness value");
        }

      } else if (commandType == "E") { // Set Effect
        currentEffect = commandData;
        if (currentEffect == "rainbow" || currentEffect == "chasing_rainbow" || currentEffect == "flashing_amber_white_sequence") {
          saveLedStateToEEPROM();
          SerialBT.println("Effect set to: " + currentEffect); // Feedback
        } else {
          currentEffect = "";
          Serial.println("Unknown effect: " + commandData);
          SerialBT.println("Unknown effect: " + commandData); // Feedback
        }
      } else if (commandType == "S") { // Set Speed
        int newSpeed = commandData.toInt();
        if (newSpeed > 0){
          animationSpeed = newSpeed;
          saveLedStateToEEPROM();
          SerialBT.println("Speed set to: " + String(animationSpeed));
        } else {
          Serial.println("Invalid speed value");
          SerialBT.println("Invalid speed value");
        }
      } else {
        Serial.println("Unknown Command type");
        SerialBT.println("Unknown Command type");
      }
    }
  }
}