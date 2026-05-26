#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Pin definitions for ESP32-C3
#define CLK_PIN   6
#define DATA_PIN  7
#define CS_PIN    5
#define MAX_DEVICES 4

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

MD_Parola Display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000); // HKT UTC+8

String ssid = "YOUR_SSID";
String password = "YOUR_PASSWORD";

String warningText = "";
unsigned long lastWarningUpdate = 0;

void setup() {
  Serial.begin(115200);
  Display.begin();
  Display.setIntensity(8);
  Display.setScrollSpacing(0);
  Display.setTextAlignment(PA_CENTER);

  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  timeClient.begin();
  updateWarnings();
}

void loop() {
  timeClient.update();

  String timeStr = timeClient.getFormattedTime().substring(0,5); // HH:MM

  if (Display.isAnimationDone()) {
    if (warningText.length() > 0 && millis() - lastWarningUpdate > 15000) { // Scroll warning every 15s
      Display.displayClear();
      Display.displayScroll(warningText.c_str(), PA_LEFT, PA_SCROLL_LEFT, 80);
    } else {
      Display.displayClear();
      Display.print(timeStr.c_str());
      delay(1000);
    }
  }
  Display.displayAnimate();

  if (millis() - lastWarningUpdate > 300000) { // Update warnings every 5 minutes
    updateWarnings();
  }
}

void updateWarnings() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=warnsum&lang=en");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      JsonDocument doc;
      deserializeJson(doc, payload);

      warningText = "";

      // Check for key warnings
      if (doc.containsKey("WTCSGNL")) { // Tropical Cyclone
        warningText += "TYPHON WARNING ";
      }
      if (doc.containsKey("WRAIN")) { // Rainstorm
        String type = doc["WRAIN"]["type"];
        warningText += type + " RAINSTORM WARNING ";
      }
      if (doc.containsKey("WHOT")) {
        warningText += "VERY HOT ";
      }

      if (warningText.length() == 0) {
        warningText = "NO WARNINGS";
      }
    }
    http.end();
  }
  lastWarningUpdate = millis();
}