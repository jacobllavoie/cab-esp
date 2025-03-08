// bluetooth_handler.h

#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include <BluetoothSerial.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"
#include "globals.h"

// --- Configuration ---
#define BLUETOOTH_NAME "cab-ESP"
#define SERIAL_BAUD_RATE 115200

// --- Global Variables ---
extern BluetoothSerial SerialBT;
extern Adafruit_NeoPixel leds;
extern String currentEffect;
extern int brightness;
extern byte currentColorRed;
extern byte currentColorGreen;
extern byte currentColorBlue;
extern byte currentColorWhite;
extern int animationSpeed;
extern const ColorDefinition colorTable[];

// --- Function Prototypes ---
void setupBluetooth();
void handleCommandInput();
void processCommand(String command);
void displayHelp();
void applyLedState();
void rainbow(int brightness);
void chasingRainbow(int brightness);
void flashingAmberWhiteSequence(int brightness);
uint32_t WheelSmooth(byte pos);
void saveLedStateToEEPROM();
#endif