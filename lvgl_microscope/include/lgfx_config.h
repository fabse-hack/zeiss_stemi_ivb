// LovyanGFX configuration for ESP32 S2 Mini + ST7735S 80x160

#ifndef LGFX_CONFIG_H_
#define LGFX_CONFIG_H_

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7735S _panel;
    lgfx::Bus_SPI _bus;
    lgfx::Light_PWM _light;

public:
    LGFX()
    {
        {
            auto cfg = _bus.config();
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 20000000;
            cfg.freq_read = 16000000;
            cfg.pin_sclk = 36;
            cfg.pin_mosi = 35;
            cfg.pin_miso = -1;
            cfg.pin_dc = 37;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        {
            auto cfg = _panel.config();
            cfg.pin_cs = 34;
            cfg.pin_rst = 38;
            cfg.pin_busy = -1;
            cfg.panel_width = 80;
            cfg.panel_height = 160;
            cfg.memory_width = 80;
            cfg.memory_height = 160;
            // Panel RAM-to-glass alignment:
            // Increase offset_x to move content to the right on the panel.
            // (+2px)
            cfg.offset_x = 15;
            // Increase offset_y to move content down on the panel.
            cfg.offset_y = -5;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = false;
            cfg.rgb_order = true;
            _panel.config(cfg);
        }

        {
            auto cfg = _light.config();
            cfg.pin_bl = 33;
            cfg.invert = false;
            cfg.freq = 5000;
            cfg.pwm_channel = 7;
            _light.config(cfg);
            _panel.setLight(&_light);
        }

        setPanel(&_panel);
    }
};

#endif // LGFX_CONFIG_H_
