// wifi_handler.cpp

#include <WiFi.h>
#include "config.h"
#include "globals.h"

void setupWifi() {
  Serial.print("Connecting to ");
  DEBUG_PRINTLN(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("WiFi connected.");
  Serial.print("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}