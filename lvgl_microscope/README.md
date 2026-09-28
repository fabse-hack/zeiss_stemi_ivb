# Microscope Crosshair Display

LVGL firmware for showing a red alignment crosshair on a display used with the Zeiss Stemi IVb microscope. The intended controller is an ESP32-S3 Mini.

## Current firmware

`src/main.cpp` draws a red reticle on a black background: horizontal and vertical lines, plus three concentric circles. LVGL handles the UI and LovyanGFX drives the display.

The checked-in configuration does **not** yet match the stated hardware:

- `platformio.ini` selects `lolin_s2_mini` (ESP32-S2), not ESP32-S3 Mini.
- `include/lgfx_config.h` configures an ST7735S 80×160 SPI TFT, not an OLED.
- Display pins in `include/lgfx_config.h` are GPIO 35, 36, 37, 34, 38 and 33, based on the old ESP32-S2 setup.

Confirm the actual display controller, resolution, interface and ESP32-S3 Mini pinout before flashing. Update the PlatformIO board and display configuration to match the hardware. Do not assume the current S2 pin assignments are valid on the S3 Mini.

## Software and dependencies

- PlatformIO with Arduino framework
- LVGL 8.3 (`lvgl/lvgl@^8.3.11`)
- LovyanGFX (`lovyan03/LovyanGFX@^1.1.16`)
- ArduinoJson (`bblanchon/ArduinoJson@^6.21.2`, currently not used by `src/main.cpp`)

LVGL configuration lives in `include/lv_conf.h`. Display configuration lives in `include/lgfx_config.h`.

## Build and upload

Current PlatformIO environment still targets the old S2 configuration. Use only after updating and verifying the environment and pins for the connected hardware:

```sh
pio run -e lolin_s2_mini
pio run -e lolin_s2_mini -t upload
pio device monitor -b 115200
```

The firmware writes setup and display status to the serial monitor at 115200 baud.

## Project files

- `src/main.cpp` — LVGL initialization, display callbacks, and crosshair UI
- `include/lgfx_config.h` — LovyanGFX panel, SPI, backlight and pin configuration
- `include/lv_conf.h` — LVGL configuration
- `platformio.ini` — PlatformIO board, framework and library dependencies
- `DISPLAY_SETUP.md` — older ST7735S / ESP32-S2 wiring notes; verify before reuse
