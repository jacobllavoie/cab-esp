// globals.h

#ifndef GLOBALS_H
#define GLOBALS_H

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
// Decide whether to use the framework BluetoothSerial or provide a harmless stub.
// Some targets (ESP32-C6) may have the BluetoothSerial header present but the
// underlying chip doesn't support Classic BT; in that case the header doesn't
// define the class. We check the SOC caps and sdkconfig macros first.
#include "sdkconfig.h"
#include "soc/soc_caps.h"

#if defined(SOC_BT_SUPPORTED) && SOC_BT_SUPPORTED && defined(CONFIG_BT_ENABLED) && defined(CONFIG_BLUEDROID_ENABLED)
#  include <BluetoothSerial.h>
#else
// Minimal stub for builds that don't provide BluetoothSerial or when BT is not supported
// by the SoC. Provides the small subset of the API used in this project so the
// code can compile on non-BT targets.
class BluetoothSerial {
public:
    BluetoothSerial() {}
    bool begin(const char* /*name*/ = nullptr) { return false; }
    int available() { return 0; }
    int read() { return -1; }
    size_t write(uint8_t /*c*/) { return 0; }
    void flush() {}
    void print(const String &/*s*/) {}
    void print(const char* /*s*/) {}
    void println(const String &/*s*/) {}
    void println(const char* /*s*/) {}
};
#endif

extern Adafruit_NeoPixel leds;
extern String currentEffect;
extern byte currentColorRed;
extern byte currentColorGreen;
extern byte currentColorBlue;
extern byte currentColorWhite;
extern int animationSpeed;
extern int brightness;
extern BluetoothSerial SerialBT;
// Heartbeat addressable pixel (single LED)
extern Adafruit_NeoPixel heartbeatPixel;
extern bool heartbeatPixelOn;
// Classic Bluetooth removed; BLE used instead

uint32_t WheelSmooth(byte pos);

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
void rebootESP();
void displayStatus();

#endif