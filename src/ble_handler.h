#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <Arduino.h>

void setupBLE();
void processBLECommand(String command);
// Send a notification to any connected BLE client (if present)
void bleNotify(const String& msg);

#endif
