#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <FastLED.h>

// ================== CONFIG ==================
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4          // 4 modules = 32x8 matrix (change if needed)
#define CLK_PIN   6
#define DATA_PIN  7
#define CS_PIN    5

#define LED_PIN   8            // WS2812B RGB LED
#define NUM_LEDS  1

// ===========================================

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
CRGB leds[NUM_LEDS];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000); // Hong Kong UTC+8

String currentWarning = "";
uint32_t lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB::Black;
  FastLED.show();

  P.begin();
  P.setIntensity(8);
  P.setTextEffect(PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  P.setSpeed(80);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  timeClient.begin();
  fetchHKOWarnings();   // Initial fetch
}

void loop() {
  timeClient.update();

  // Update warnings every 5 minutes
  if (millis() - lastUpdate > 300000 || lastUpdate == 0) {
    fetchHKOWarnings();
    lastUpdate = millis();
  }

  String displayText = getTimeString();
  if (currentWarning.length() > 0) {
    displayText += "   " + currentWarning;
  }

  P.print(displayText);

  if (P.displayAnimate()) {
    P.displayReset();
  }

  delay(50);
}

String getTimeString() {
  int h = timeClient.getHours();
  int m = timeClient.getMinutes();
  char buf[10];
  sprintf(buf, "%02d:%02d", h, m);
  return String(buf);
}

void fetchHKOWarnings() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin("https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=warnsum&lang=en");
  
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    JsonDocument doc;
    deserializeJson(doc, payload);

    String typhoon = "";
    String rain = "";

    // Typhoon Warning
    if (doc.containsKey("WTCSGNL")) {
      String code = doc["WTCSGNL"]["code"].as<String>();
      if (code.length() > 2) {
        typhoon = "T" + code.substring(2);
      }
    }

    // Rainstorm Warning
    if (doc.containsKey("WRAIN")) {
      String type = doc["WRAIN"]["type"].as<String>();
      if (type == "Amber") rain = "Y.Rain";
      else if (type == "Red") rain = "R.Rain";
      else if (type == "Black") rain = "B.Rain";
    }

    // Priority: Typhoon first
    if (typhoon != "") {
      currentWarning = typhoon;
    } else if (rain != "") {
      currentWarning = rain;
    } else {
      currentWarning = "";
    }

    updateLEDIndicator(typhoon);
  }
  http.end();
}

void updateLEDIndicator(String typhoon) {
  if (typhoon == "T1") {
    leds[0] = CRGB::Blue;
  } else if (typhoon == "T3") {
    leds[0] = CRGB::Yellow;
  } else if (typhoon == "T8") {
    leds[0] = CRGB::Orange;
  } else if (typhoon == "T9" || typhoon == "T10") {
    leds[0] = CRGB::Red;
  } else {
    leds[0] = CRGB::Black;
  }
  FastLED.show();
}
