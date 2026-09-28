# LVGL auf ESP32 S2 Mini mit ST7735S Display

## Hardware-Verbindung

### Pinout ESP32 S2 Mini → ST7735S Display

| ESP32 S2 Mini | ST7735S Display | Funktion |
|---------------|-----------------|----------|
| **GPIO 35**   | **SDA / MOSI**  | Serial Data In (Datenlinie) |
| **GPIO 36**   | **SCL / CLK**   | Serial Clock (Taktsignal) |
| **GPIO 34**   | **CS**          | Chip Select (Aktivierung) |
| **GPIO 37**   | **DC / A0**     | Data/Command (Datenbefehl) |
| **GPIO 38**   | **RST**         | Reset |
| **GPIO 33**   | **BL**          | Backlight (optional) |
| **3.3V**      | **VCC**         | Stromversorgung |
| **GND**       | **GND**         | Masse |

### Schaltplan

```
ESP32 S2 Mini
┌─────────────────────┐
│                     │
│  GPIO 35 (MOSI) ──────────► SDA/MOSI
│  GPIO 36 (CLK)  ──────────► SCL/CLK
│  GPIO 34 (CS)   ──────────► CS
│  GPIO 37 (DC)   ──────────► DC/A0
│  GPIO 38 (RST)  ──────────► RST
│  GPIO 33 (BL)   ──────────► BL (optional, für PWM Helligkeit)
│                     │
│  3.3V           ──────────► VCC
│  GND            ──────────► GND
│                     │
└─────────────────────┘
          ◆
          │
        [ST7735S]
        SPI Display
        240x135px
```

## Wichtig!

### Stromversorgung
- **VCC**: 3.3V (nicht 5V! Das zerstört das Display!)
- Ggf. eine kleine 100nF Keramik-Kondensator zwischen VCC und GND nahe dem Display

### Widerstände (optional aber empfohlen)
- Pull-up Widerstände auf CS, DC für Stabilität (z.B. 10kΩ zu 3.3V)
- Das ist nicht zwingend erforderlich, verbessert aber die Stabilität

## Software-Setup

### 1. PlatformIO Projekt initialisieren
```bash
platformio init --board lolin_s2_mini --framework arduino
```

### 2. Abhängigkeiten sind bereits in `platformio.ini` definiert:
- **TFT_eSPI** - Display-Treiber
- **LVGL** - UI-Framework
- **ArduinoJson** - Für zukünftige Konfigurationen

### 3. Code kompilieren und hochladen
```bash
platformio run --target upload
```

### 4. Serial Monitor
```bash
platformio device monitor --speed 115200
```

## Features des bereitgestellten Codes

✅ **LVGL Initialisierung** mit Puffern  
✅ **Display-Treiber** für ST7735S  
✅ **Demo UI** mit:
   - Titel und Untertitel
   - Button
   - Slider
   - Schöne Farben

✅ **Tick-Timer** für LVGL Animations-System  
✅ **Serial Debug Output** für Fehlersuche

## Problembehebung

### Display zeigt nichts
1. Überprüfe alle Verkabelungen (besonders GND und VCC)
2. Überprüfe Serial Monitor auf Fehler
3. Versuche, das `tft.setRotation(3)` in `main.cpp` zu ändern zu 0, 1 oder 2
4. CS und DC müssen korrekt verdrahtet sein

### Falsche Farben
- Die Farben hängen vom ST7735S Offset ab. Falls Farben komisch aussehen:
  - In [include/tft_setup.h](include/tft_setup.h): `#define ST7735_GREENTAB` oder `#define ST7735_REDTAB` versuchen

### LVGL Speicherfehler
- Erhöhe `LV_MEM_SIZE` in [include/lv_conf.h](include/lv_conf.h) (64 KB → 128 KB)
- Reduziere `LVGL_BUF_SIZE` in `main.cpp` wenn RAM zu knapp wird

### SPI-Geschwindigkeit anpassen
Falls das Display flackert:
- In [include/tft_setup.h](include/tft_setup.h): `#define SPI_FREQUENCY` auf 20000000 (20 MHz) reduzieren

## Weitere LVGL Widgets

Der Code enthält bereits Beispiele für:
- **lv_label** - Text
- **lv_btn** - Buttons
- **lv_slider** - Schieberegler
- **lv_style** - Styling

Weitere Widgets: https://docs.lvgl.io/8.3/widgets/index.html

## Tipps

- **Helligkeit regeln**: GPIO 33 mit PWM ansteuern für BL Pin
- **Externe Schriftarten**: LVGL unterstützt Custom Fonts
- **Performance**: Bei Speicherengpässen `LVGL_BUF_SIZE` reduzieren

---
**Viel Spaß mit deinem Display! 🎨**
