// lv_conf.h - LVGL configuration for ESP32 S2 Mini

#ifndef LV_CONF_H
#define LV_CONF_H

// ========================================
// Display and color
// ========================================
#define LV_HOR_RES_MAX 80
#define LV_VER_RES_MAX 160
#define LV_COLOR_DEPTH 16  // 16-bit colors

// ========================================
// Memory configuration
// ========================================
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (64U * 1024U)  // 64 KB

// ========================================
// Refresh and performance
// ========================================
#define LV_DISP_DEF_REFR_PERIOD 10  // ms

// ========================================
// Object configuration
// ========================================
#define LV_OBJ_DRAW_PART_HINT_CACHED 0

// ========================================
// Antialias
// ========================================
#define LV_ANTIALIAS 1

// ========================================
// Animation
// ========================================
#define LV_USE_ANIMATION 1
#define LV_ANIM_SPEED_0 0
#define LV_ANIM_SPEED_SLOWEST 10
#define LV_ANIM_SPEED_SLOW 20
#define LV_ANIM_SPEED_MID 30
#define LV_ANIM_SPEED_FAST 40
#define LV_ANIM_SPEED_FASTEST 50

// ========================================
// Enable widgets
// ========================================
#define LV_USE_LABEL 1
#define LV_USE_BTN 1
#define LV_USE_SLIDER 1
#define LV_USE_CONT 1
#define LV_USE_PAGE 1
#define LV_USE_MSGBOX 1
#define LV_USE_SPINBOX 1
#define LV_USE_TEXTAREA 1
#define LV_USE_CANVAS 1

// ========================================
// Fonts
// ========================================
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

#endif // LV_CONF_H
