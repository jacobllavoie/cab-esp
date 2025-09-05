// wifi_handler.cpp

#include <WiFi.h>
#include <WebServer.h> // <-- Use WebServer instead of Async
#include <ElegantOTA.h>
#include "config.h"
#include "globals.h"

// Create WebServer object on port 80
WebServer server(80);

void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // A simple page to confirm the server is running
  server.on("/", []() {
    server.send(200, "text/plain", "Cab-ESP is running!");
  });

  // Start ElegantOTA
  ElegantOTA.begin(&server);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started.");
}

// This function must be called in the main loop
void handleWebServer() {
  server.handleClient();
}