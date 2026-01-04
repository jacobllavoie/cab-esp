
#include "ble_handler.h"
#include "globals.h"
#include "config.h"

#include <NimBLEDevice.h>
#include <esp_task_wdt.h>

static const char* BLE_DEVICE_NAME = "cab-ESP-BLE";
// Unique service/characteristic UUIDs for cab-ESP BLE
// Base UUID provided by user; variants 1/2/3 used for service/write/notify respectively
static const char* SERVICE_UUID = "edc01af1-5f53-4ee0-83df-e6888b252f6e";
static const char* CHAR_UUID_WRITE = "edc01af2-5f53-4ee0-83df-e6888b252f6e";
static const char* CHAR_UUID_NOTIFY = "edc01af3-5f53-4ee0-83df-e6888b252f6e";

static NimBLECharacteristic* pWriteChar = nullptr;
static NimBLECharacteristic* pNotifyChar = nullptr;
static String incomingBuffer = "";

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
    Serial.print("BLE client connected: ");
    Serial.println(connInfo.getAddress().toString().c_str());
  }
  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
    Serial.print("BLE client disconnected (reason ");
    Serial.print(reason);
    Serial.print("): ");
    Serial.println(connInfo.getAddress().toString().c_str());
  }
};

class CommandCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
    std::string val = pCharacteristic->getValue();
    // Debug: print the raw write chunk so partial writes are visible in the serial log
    Serial.print("BLE write chunk (len=");
    Serial.print(val.size());
    Serial.print("): '");
    // Print as-is; non-printable bytes may be omitted by the monitor filter
    for (size_t j = 0; j < val.size(); ++j) Serial.print(val[j]);
    Serial.println("'");
    for (size_t i = 0; i < val.size(); ++i) {
      char c = val[i];
      // Accept both actual newline characters and escaped sequences ("\\n" / "\\r")
      if (c == '\\' && (i + 1) < val.size() && (val[i + 1] == 'n' || val[i + 1] == 'r')) {
        // treat escaped \n or \r as a line terminator
        incomingBuffer.trim();
        if (incomingBuffer.length() > 0) {
          Serial.print("Received from BLE: ");
          Serial.println(incomingBuffer);
          // Use the BLE-specific wrapper so we can ack/notify if desired
          processBLECommand(incomingBuffer);
        }
        incomingBuffer = "";
        ++i; // skip the 'n' or 'r' we just processed
      } else if (c == '\n' || c == '\r') {
        incomingBuffer.trim();
        if (incomingBuffer.length() > 0) {
          Serial.print("Received from BLE: ");
          Serial.println(incomingBuffer);
          // Use the BLE-specific wrapper so we can ack/notify if desired
          processBLECommand(incomingBuffer);
        }
        incomingBuffer = "";
      } else {
        incomingBuffer += c;
      }
    }
  }
};

void bleNotify(const String& msg) {
  if (pNotifyChar) {
    pNotifyChar->setValue((uint8_t*)msg.c_str(), msg.length());
    pNotifyChar->notify(true);
  }
}

void setupBLE() {
  // Disable watchdog during BLE initialization which can take some time
  disableCore0WDT();
  
  NimBLEDevice::init(BLE_DEVICE_NAME);
  NimBLEServer* pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  NimBLEService* pService = pServer->createService(SERVICE_UUID);

  pWriteChar = pService->createCharacteristic(
    CHAR_UUID_WRITE,
    NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR
  );
  pWriteChar->setCallbacks(new CommandCallbacks());

  pNotifyChar = pService->createCharacteristic(
    CHAR_UUID_NOTIFY,
    NIMBLE_PROPERTY::NOTIFY
  );

  pService->start();
  NimBLEAdvertising* pAdv = NimBLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->start();

  Serial.print("BLE started: ");
  Serial.println(BLE_DEVICE_NAME);
  DEBUG_PRINTLN("BLE ready");
  
  // Re-enable watchdog
  enableCore0WDT();
  
  delay(100); // Small delay to let BLE fully initialize
}

void processBLECommand(String command) {
  // Forward to the serial command processor so the same command parser is reused
  processSerialCommand(command);
  // Optionally send acknowledgement
  bleNotify("OK\n");
}
