#ifndef GLOBALS_H
#define GLOBALS_H

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

extern Adafruit_NeoPixel leds;
extern String currentEffect;
extern byte currentColorRed;
extern byte currentColorGreen;
extern byte currentColorBlue;
extern byte currentColorWhite;
extern int animationSpeed;
extern int brightness;

uint32_t WheelSmooth(byte pos); // Declare WheelSmooth here.

struct Effect {
    String name;
    void (*function)(int);
};

extern Effect effects[];
extern int numberOfEffects;

void readLedStateFromEEPROM();
void saveLedStateToEEPROM();
void applyLedState();
void rainbow(int brightness);
void chasingRainbow(int brightness);
void flashingAmberWhiteSequence(int brightness);
void displayHelp();
void processSerialCommand(String command);

#endif