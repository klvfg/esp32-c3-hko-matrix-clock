# ESP32-C3 HKO Matrix Clock

A WiFi NTP clock with MAX7219 LED matrix display that shows current time and Hong Kong Observatory typhoon/rainstorm warnings.

## Features
- Real-time clock via NTP
- Displays Hong Kong weather warnings (Typhoon, Rainstorm, etc.)
- Scrolling text for warnings
- ESP32-C3 Super Mini compatible

## Hardware
- ESP32-C3 Super Mini
- MAX7219 4x or 8x LED Matrix (32x8 or larger)

## Wiring (typical for ESP32-C3)
- DIN (Data) → GPIO 7
- CLK → GPIO 6
- CS → GPIO 5
- VCC → 5V
- GND → GND

## Libraries
- MD_Parola
- MD_MAX72xx
- WiFi
- NTPClient
- ArduinoJson

## How to use
1. Update WiFi credentials
2. Upload to ESP32-C3
3. Enjoy your smart clock!