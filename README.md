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

---

## Setup and maintenance notes

> These notes were added from the current source tree on 2026-11-18. The README is kept in the original repository; no generated repository is required.

### Overview

Hong Kong Observatory weather display and warning-light experiment.

### Entry points

- `src/main.cpp`

### Platform and tooling

This project was detected as using: **ESP32 / ESP32-C3**, **Arduino framework**, **PlatformIO**.

### Detected libraries

The source references: `MD_Parola`, `MD_MAX72XX`, `FastLED`, `ArduinoJson`, `NTPClient`. Install matching versions before compiling.

### Hardware and wiring

The source defines or uses these relevant pins:
- `CLK_PIN=6`
- `DATA_PIN=7`
- `CS_PIN=5`
- `LED_PIN=8`
- `NUM_LEDS=1`

Pin labels vary by board. Verify the board pinout and use current-limiting resistors for discrete LEDs. I2C and SPI modules must share ground with the controller.

### Build with PlatformIO

From the repository root:

```sh
pio run
pio run -t upload
pio device monitor
```

### Wi-Fi configuration

Wi-Fi identifiers or passwords appear in source. Move them to a local ignored configuration file before sharing the repository.

### Network services

Detected endpoints:
- `https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=warnsum&lang=en`

Network services can change or be unavailable. Handle failed requests and avoid embedding private tokens in source.

### Verification

No hardware build was run in this documentation pass. After installing the board package and libraries, compile for the selected board and test the display, network connection, and output pins on the actual hardware.
