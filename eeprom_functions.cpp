// eeprom_functions.cpp

#include "config.h"
#include "globals.h"

void saveLedStateToEEPROM() {
  Serial.println("saveLedStateToEEPROM: Starting");

  // Save string length and characters
  int effectLength = currentEffect.length();
  Serial.print("saveLedStateToEEPROM: Effect Length = ");
  Serial.println(effectLength);
  EEPROM.write(EEPROM_EFFECT_ADDR, effectLength);
  for (int i = 0; i < effectLength; i++) {
    EEPROM.write(EEPROM_EFFECT_ADDR + 1 + i, currentEffect[i]);
  }
  Serial.print("saveLedStateToEEPROM: Effect = ");
  Serial.println(currentEffect);

  // Save other LED state variables
  Serial.print("saveLedStateToEEPROM: Brightness = ");
  Serial.println(brightness);
  EEPROM.write(EEPROM_BRIGHTNESS_ADDR, brightness);
  Serial.print("saveLedStateToEEPROM: Red = ");
  Serial.println(currentColorRed);
  EEPROM.write(EEPROM_RED_ADDR, currentColorRed);
  Serial.print("saveLedStateToEEPROM: Green = ");
  Serial.println(currentColorGreen);
  EEPROM.write(EEPROM_GREEN_ADDR, currentColorGreen);
  Serial.print("saveLedStateToEEPROM: Blue = ");
  Serial.println(currentColorBlue);
  EEPROM.write(EEPROM_BLUE_ADDR, currentColorBlue);
  Serial.print("saveLedStateToEEPROM: White = ");
  Serial.println(currentColorWhite);
  EEPROM.write(EEPROM_WHITE_ADDR, currentColorWhite);
  Serial.print("saveLedStateToEEPROM: Speed = ");
  Serial.println(animationSpeed);
  EEPROM.write(EEPROM_SPEED_ADDR, animationSpeed);
  EEPROM.commit();

  Serial.println("saveLedStateToEEPROM: Finished");
}

void readLedStateFromEEPROM() {
  // Load string length and characters
  int effectLength = EEPROM.read(EEPROM_EFFECT_ADDR);
  if (effectLength > 0) {
    currentEffect = "";
    for (int i = 0; i < effectLength; i++) {
      currentEffect += (char)EEPROM.read(EEPROM_EFFECT_ADDR + 1 + i);
    }
  } else {
    currentEffect = ""; // No effect stored
  }

  // Load other LED state variables
  brightness = EEPROM.read(EEPROM_BRIGHTNESS_ADDR);
  currentColorRed = EEPROM.read(EEPROM_RED_ADDR);
  currentColorGreen = EEPROM.read(EEPROM_GREEN_ADDR);
  currentColorBlue = EEPROM.read(EEPROM_BLUE_ADDR);
  currentColorWhite = EEPROM.read(EEPROM_WHITE_ADDR);
  animationSpeed = EEPROM.read(EEPROM_SPEED_ADDR);

  // Constrain values
  brightness = constrain(brightness, 0, 255);
  animationSpeed = constrain(animationSpeed, 1, 255);
}

void applyLedState() {
  Serial.print("applyLedState: R=");
  Serial.print(currentColorRed);
  Serial.print(", G=");
  Serial.print(currentColorGreen);
  Serial.print(", B=");
  Serial.print(currentColorBlue);
  Serial.print(", W=");
  Serial.println(currentColorWhite);

  if (currentEffect.equalsIgnoreCase("rainbow")) {
    rainbow(brightness);
  } else if (currentEffect.equalsIgnoreCase("chasing_rainbow")) {
    chasingRainbow(brightness);
  } else if (currentEffect.equalsIgnoreCase("flashing_amber_white_sequence")) {
    flashingAmberWhiteSequence(brightness);
  } else if (currentColorRed != 0 || currentColorGreen != 0 || currentColorBlue != 0 || currentColorWhite != 0) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds.setPixelColor(j, (currentColorRed * brightness) / 255, (currentColorGreen * brightness) / 255, (currentColorBlue * brightness) / 255, currentColorWhite);
    }
    leds.show();
  }

  leds.setBrightness(brightness);
  leds.show();
}