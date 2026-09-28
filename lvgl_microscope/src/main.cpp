#include <Arduino.h>
#include "lgfx_config.h"
#include <lvgl.h>


// ========================================
// LovyanGFX Instanz
// ========================================
LGFX tft;

// ========================================
// LVGL Puffer und Display
// ========================================
#define DISPLAY_ROTATION 0  // 0=0°, 1=90°, 2=180°, 3=270°
#define LVGL_BUF_SIZE (80 * 160 / 10)
#define UI_LANDSCAPE 1


static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf1[LVGL_BUF_SIZE];
static lv_color_t buf2[LVGL_BUF_SIZE];
static lv_disp_drv_t disp_drv;

static uint8_t get_stable_rotation(uint8_t requested)
{
    return (requested % 2 == 1) ? 0 : requested;
}

// ========================================
// Display Callback (wird von LVGL aufgerufen)
// ========================================
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if UI_LANDSCAPE
    tft.startWrite();
    uint32_t idx = 0;
    for (int32_t y = area->y1; y <= area->y2; y++)
    {
        for (int32_t x = area->x1; x <= area->x2; x++)
        {
            int32_t phys_x = 79 - y;
            int32_t phys_y = x;
            tft.writePixel(phys_x, phys_y, color_p[idx].full);
            idx++;
        }
    }
    tft.endWrite();
#else
    tft.pushImage(area->x1, area->y1, w, h, (lgfx::rgb565_t *)color_p);
#endif
    
    lv_disp_flush_ready(disp);
}

// ========================================
// Tick für LVGL (bei jedem Millisekunde)
// ========================================
static void lv_tick_task(void)
{
    lv_tick_inc(1);
}

static void create_reticle(lv_obj_t *parent)
{
    const lv_coord_t w = lv_obj_get_width(parent);
    const lv_coord_t h = lv_obj_get_height(parent);
    const lv_coord_t cx = w / 2;
    const lv_coord_t cy = h / 2;

    // Simple crosshair + circle reticle for microscope alignment
    const lv_color_t reticle_col = lv_color_hex(0xFF0000);

    // Horizontal line
    lv_obj_t *hline = lv_obj_create(parent);
    lv_obj_set_size(hline, 44, 1);
    lv_obj_set_pos(hline, cx - 22, cy - 1);
    lv_obj_clear_flag(hline, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(hline, reticle_col, 0);
    lv_obj_set_style_bg_opa(hline, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(hline, 0, 0);
    lv_obj_set_style_pad_all(hline, 0, 0);
    lv_obj_set_style_radius(hline, 0, 0);

    // Vertical line
    lv_obj_t *vline = lv_obj_create(parent);
    lv_obj_set_size(vline, 1, 44);
    lv_obj_set_pos(vline, cx - 1, cy - 22);
    lv_obj_clear_flag(vline, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(vline, reticle_col, 0);
    lv_obj_set_style_bg_opa(vline, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(vline, 0, 0);
    lv_obj_set_style_pad_all(vline, 0, 0);
    lv_obj_set_style_radius(vline, 0, 0);

    // Outer circles
    lv_obj_t *circle_outer_small = lv_obj_create(parent);
    lv_obj_set_size(circle_outer_small, 22, 22);
    lv_obj_set_pos(circle_outer_small, cx - 11, cy - 11);
    lv_obj_clear_flag(circle_outer_small, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(circle_outer_small, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(circle_outer_small, reticle_col, 0);
    lv_obj_set_style_border_width(circle_outer_small, 1, 0);
    lv_obj_set_style_radius(circle_outer_small, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_pad_all(circle_outer_small, 0, 0);

    lv_obj_t *circle_outer_large = lv_obj_create(parent);
    lv_obj_set_size(circle_outer_large, 44, 44);
    lv_obj_set_pos(circle_outer_large, cx - 22, cy - 22);
    lv_obj_clear_flag(circle_outer_large, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(circle_outer_large, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(circle_outer_large, reticle_col, 0);
    lv_obj_set_style_border_width(circle_outer_large, 1, 0);
    lv_obj_set_style_radius(circle_outer_large, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_pad_all(circle_outer_large, 0, 0);

    // Inner circle
    lv_obj_t *circle_inner = lv_obj_create(parent);
    lv_obj_set_size(circle_inner, 10, 10);
    lv_obj_set_pos(circle_inner, cx - 5, cy - 5);
    lv_obj_clear_flag(circle_inner, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(circle_inner, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(circle_inner, reticle_col, 0);
    lv_obj_set_style_border_width(circle_inner, 1, 0);
    lv_obj_set_style_radius(circle_inner, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_pad_all(circle_inner, 0, 0);
}

// ========================================
// LVGL Initialisierung
// ========================================
void lvgl_init()
{
    lv_init();

    // Draw-Buffer initialisieren
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LVGL_BUF_SIZE);

    // Display-Driver initialisieren
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.full_refresh = 1;
#if UI_LANDSCAPE
    disp_drv.hor_res = 160;
    disp_drv.ver_res = 80;
#else
    disp_drv.hor_res = 80;
    disp_drv.ver_res = 160;
#endif
    
    lv_disp_drv_register(&disp_drv);

    // Timer für Ticks
    hw_timer_t *timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &lv_tick_task, true);
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
}

// ========================================
// Demo UI mit LVGL erstellen
// ========================================
void create_demo_ui()
{
    lv_obj_t *scr = lv_scr_act();
    lv_coord_t screen_w = lv_obj_get_width(scr);

    // Screen Background
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    // lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

    // Microscope targeting reticle
    create_reticle(scr);
}

// ========================================
// Setup
// ========================================
void setup()
{
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=== LVGL ST7735S Test ===\n");

    // TFT initialisieren
    Serial.println("Initialisiere TFT...");
    tft.init();
    const uint8_t applied_rotation = get_stable_rotation(DISPLAY_ROTATION);
    tft.setRotation(applied_rotation);
    if (applied_rotation != DISPLAY_ROTATION)
    {
        Serial.printf("Hinweis: Rotation %u ist mit aktueller ST7735-Konfig instabil, nutze %u\n",
                      DISPLAY_ROTATION, applied_rotation);
    }
    tft.setBrightness(50);
    // tft.fillScreen(TFT_BLACK);

    Serial.printf("Display size after rotation: %ux%u\n", tft.width(), tft.height());

    // Quick color test to verify the panel is responding
    // tft.fillScreen(TFT_RED);
    // delay(200);
    // tft.fillScreen(TFT_GREEN);
    // delay(200);
    // tft.fillScreen(TFT_BLUE);
    // delay(200);
    // tft.fillScreen(TFT_BLACK);
    
    Serial.println("TFT initialisiert!");

    // LVGL initialisieren
    Serial.println("Initialisiere LVGL...");
    lvgl_init();
    
    Serial.println("LVGL initialisiert!");

    // Clear once after LVGL init to avoid any leftover pixels
    // tft.fillScreen(TFT_BLACK);

    // Demo UI erstellen
    Serial.println("Erstelle Demo UI...");
    create_demo_ui();

    // Force a full refresh once
    lv_obj_invalidate(lv_scr_act());
    
    Serial.println("Setup komplett!");
}

// ========================================
// Loop
// ========================================
void loop()
{
    lv_task_handler();
    delay(5);
}