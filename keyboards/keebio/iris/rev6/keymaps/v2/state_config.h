#ifndef QMK_USERSPACE_STATE_CONFIG_H
#define QMK_USERSPACE_STATE_CONFIG_H

bool is_alt_tab_active = false;  // Tracks if the Alt-Tab overlay is currently open
uint16_t alt_tab_timer = 0;     // Timer to auto-release Alt after a timeout

uint16_t toggle_hold_timer = 0;
bool toggle_key_pressed = false;
bool toggle_threshold_met = false; // Prevents rapid flickering while holding


// RGB Colors for layers and caps lock
uint8_t colourDefault[] = {57, 184, 255}; // Light Blue
uint8_t colourGaming[]  = {255, 0, 0};    // Red

// LED Index Mapping (Iris Rev 6 specific)
int underglowLED[6] = {28, 29, 30, 31, 32, 33};
int backlightLEDinner[15] = {6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
int backlightLEDall[28] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

#endif // QMK_USERSPACE_STATE_CONFIG_H
