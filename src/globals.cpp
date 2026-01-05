// globals.cpp

#include "globals.h"

int animationSpeed = 50;
int brightness = 255;

// SerialBT removed - Classic Bluetooth conflicts with NimBLE
// Use BLE notifications via bleNotify() instead

Effect effects[] = {
  {"rainbow", rainbow},
  {"chasing_rainbow", chasingRainbow},
  {"amber_white_strobe", amberWhiteStrobe},
  {"green_white_strobe", greenWhiteStrobe}
};
int numberOfEffects = sizeof(effects) / sizeof(effects[0]);

// Indicates if Classic Bluetooth successfully started
// Classic Bluetooth removed; BLE will be used via ble_handler