#pragma once
bool is_alt_tab_active = false;  // Tracks if the Alt-Tab overlay is currently open
uint16_t alt_tab_timer = 0;     // Timer to auto-release Alt after a timeout

uint16_t toggle_hold_timer = 0;
bool toggle_key_pressed = false;
bool toggle_threshold_met = false; // Prevents rapid flickering while holding

// RGB Colors for layers and caps lock
uint8_t colourDefault[] = {57, 184, 255}; // Light Blue
uint8_t colourGaming[]  = {255, 0, 0};    // Red
