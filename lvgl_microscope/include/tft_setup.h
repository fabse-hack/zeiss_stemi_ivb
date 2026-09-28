// Project-specific TFT_eSPI setup for ESP32 S2 Mini + ST7735S

#ifndef TFT_SETUP_H_
#define TFT_SETUP_H_

// Display driver
#define ST7735_DRIVER

// Resolution
#define TFT_WIDTH  80
#define TFT_HEIGHT 160

// Panel variant (black tab module)
#define ST7735_BLACKTAB

// SPI pin configuration for ESP32 S2 Mini
#define TFT_MOSI 35  // GPIO 35 (MOSI)
#define TFT_SCLK 36  // GPIO 36 (CLK)
#define TFT_MISO -1  // Not used
#define TFT_CS   34  // GPIO 34 (Chip Select)
#define TFT_DC   37  // GPIO 37 (Data/Command)
#define TFT_RST  38  // GPIO 38 (Reset)
#define TFT_BL   33  // GPIO 33 (Backlight PWM) - optional

// SPI frequency
#define SPI_FREQUENCY 20000000  // 20 MHz

// Backlight polarity
#define TFT_BL_ON HIGH

// Font support
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

// Smooth fonts
#define SMOOTH_FONT

#endif // TFT_SETUP_H_
