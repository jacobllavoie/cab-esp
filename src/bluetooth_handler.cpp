// bluetooth_handler.cpp

#include "config.h"
#include "globals.h"

void setupBluetooth() {
  // Classic Bluetooth removed. BLE is used via ble_handler.
  DEBUG_PRINTLN("Classic Bluetooth removed; using BLE.");
}

void handleBluetoothInput() {
  // No-op. BLE writes are handled by ble_handler callbacks.
}

void processBluetoothCommand(String command) {
  // Forward to the serial parser so older callers still work.
  processSerialCommand(command);
}