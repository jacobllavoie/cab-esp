// utility_functions.cpp

#include "config.h"
#include "globals.h"

void displayStatus() {
  String statusMessage = "** Current Status **\n";
  statusMessage += "Effect: " + (currentEffect == "" ? "Solid Color" : currentEffect) + "\n";
  statusMessage += "Brightness: " + String(brightness) + "\n";
  statusMessage += "Animation Speed: " + String(animationSpeed) + "\n";
  statusMessage += "Color (R,G,B,W): " + String(currentColorRed) + "," + String(currentColorGreen) + "," + String(currentColorBlue) + "," + String(currentColorWhite) + "\n";
  
  Serial.print(statusMessage);
  SerialBT.print(statusMessage);
}

void rebootESP() {
  DEBUG_PRINTLN("Rebooting ESP...");
  DEBUG_PRINTLN_BT("Rebooting ESP...");
  delay(100); // Short delay to allow messages to send
  ESP.restart();
}

void displayHelp() {
  DEBUG_PRINTLN("**Help Menu**");
  DEBUG_PRINTLN("C,<color_name>: Set color (e.g., C,red)");
  DEBUG_PRINTLN("B,<brightness>: Set brightness (0-255) (e.g., B,128)");
  DEBUG_PRINTLN("E,<effect_name>: Set effect (rainbow, chasing_rainbow, flashing_amber, flashing_amber_white) (e.g., E,rainbow)");
  DEBUG_PRINTLN("S,<speed>: Set speed (ms) (e.g., S,20)");
  DEBUG_PRINTLN("H: Display help");
  DEBUG_PRINTLN("R: Reboot the device");
  DEBUG_PRINTLN("STATUS: Display current status");

  DEBUG_PRINTLN_BT("**Help Menu**");
  DEBUG_PRINTLN_BT("C,<color_name>: Set color (e.g., C,red)");
  DEBUG_PRINTLN_BT("B,<brightness>: Set brightness (0-255) (e.g., B,128)");
  DEBUG_PRINTLN_BT("E,<effect_name>: Set effect (rainbow, chasing_rainbow, flashing_amber, flashing_amber_white) (e.g., E,rainbow)");
  DEBUG_PRINTLN_BT("S,<speed>: Set speed (ms) (e.g., S,20)");
  DEBUG_PRINTLN_BT("H: Display help");
  DEBUG_PRINTLN_BT("R: Reboot the device");
  DEBUG_PRINTLN_BT("STATUS: Display current status");
}

uint32_t WheelSmooth(byte pos) {
  float r, g, b;

  if (pos < 85) {
    r = 255 - pos * 3;
    g = pos * 3;
    b = 0;
  } else if (pos < 170) {
    pos -= 85;
    r = 0;
    g = 255 - pos * 3;
    b = pos * 3;
  } else {
    pos -= 170;
    r = pos * 3;
    g = 0;
    b = 255 - pos * 3;
  }

  // Use a sine wave for smoother transitions
  r = 127.5 + 127.5 * sin(PI * (r / 255.0 - 0.5));
  g = 127.5 + 127.5 * sin(PI * (g / 255.0 - 0.5));
  b = 127.5 + 127.5 * sin(PI * (b / 255.0 - 0.5));

  return (uint32_t)((byte)r << 16 | (byte)g << 8 | (byte)b);
}