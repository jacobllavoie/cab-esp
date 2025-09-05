// wifi_handler.cpp

#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>
#include "config.h"
#include "globals.h"

const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 LED Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; background: #222; color: #eee; }
        .container { max-width: 400px; margin: 20px auto; padding: 15px; background: #333; border-radius: 8px; }
        h2 { text-align: center; }
        .control-group { margin-bottom: 15px; }
        label { display: block; margin-bottom: 5px; }
        input[type="color"], input[type="range"], select, button {
            width: 100%; padding: 10px; box-sizing: border-box; border-radius: 5px; border: none;
        }
        button { background: #007bff; color: white; cursor: pointer; }
    </style>
</head>
<body>
    <div class="container">
        <h2>LED Control</h2>
        <div class="control-group">
            <label for="effect">Effect:</label>
            <select id="effect" onchange="setEffect()">
                <option value="">Solid Color</option>
                <option value="rainbow">Rainbow</option>
                <option value="chasing_rainbow">Chasing Rainbow</option>
                <option value="flashing_amber_white_sequence">Amber/White Flash</option>
            </select>
        </div>
        <div class="control-group">
            <label for="color">Color:</label>
            <input type="color" id="color" value="#ff0000" onchange="setColor()">
        </div>
        <div class="control-group">
            <label for="brightness">Brightness:</label>
            <input type="range" id="brightness" min="0" max="255" value="255" oninput="setBrightness()">
        </div>
    </div>

    <script>
        function setEffect() {
            const effect = document.getElementById('effect').value;
            fetch(`/set?effect=${effect}`);
        }
        function setColor() {
            const color = document.getElementById('color').value.substring(1); // remove '#'
            const r = parseInt(color.substring(0,2), 16);
            const g = parseInt(color.substring(2,4), 16);
            const b = parseInt(color.substring(4,6), 16);
            fetch(`/set?color=${r},${g},${b}`);
        }
        function setBrightness() {
            const brightness = document.getElementById('brightness').value;
            fetch(`/set?brightness=${brightness}`);
        }
    </script>
</body>
</html>
)=====";

WebServer server(80);

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

  server.on("/", []() {
    server.send(200, "text/html", INDEX_HTML);
  });

  server.on("/set", []() {
    if (server.hasArg("effect")) {
      currentEffect = server.arg("effect");
      DEBUG_PRINTLN("Effect set to: " + currentEffect);
    }
    if (server.hasArg("color")) {
      String colorStr = server.arg("color");
      sscanf(colorStr.c_str(), "%hhu,%hhu,%hhu", &currentColorRed, &currentColorGreen, &currentColorBlue);
      currentEffect = ""; // Setting a color cancels an effect
      Serial.printf("Color set to: R=%d, G=%d, B=%d\n", currentColorRed, currentColorGreen, currentColorBlue);
    }
    if (server.hasArg("brightness")) {
      brightness = server.arg("brightness").toInt();
      DEBUG_PRINTLN("Brightness set to: " + String(brightness));
    }
    
    saveLedStateToEEPROM();
    applyLedState();
    server.send(200, "text/plain", "OK");
  });

  // Start ElegantOTA
  ElegantOTA.begin(&server);
  
  // Start the server
  server.begin();
  DEBUG_PRINTLN("HTTP server started.");
}

// This function must be called in the main loop
void handleWebServer() {
  server.handleClient();
}