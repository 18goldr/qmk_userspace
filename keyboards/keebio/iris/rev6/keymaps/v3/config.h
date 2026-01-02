#pragma once

// Enables the 'get_tapping_term' function in your keymap.c.
// This allows you to set different timings for different keys (like your home row mods).
#define TAPPING_TERM_PER_KEY

// The default time (in milliseconds) a key must be held to register as a modifier.
#define TAPPING_TERM 160

// Tells QMK to look at the EEPROM (on-board memory) to determine which half is left/right.
// This allows you to plug the USB cable into either side of the Iris.
#define EE_HANDS

// Ensures that both halves of the split keyboard stay in sync regarding which
// layer is currently active, so RGB indicators work correctly on both sides.
#define SPLIT_LAYER_STATE_ENABLE

// ARM-based Macs (M1/M2/M3) can sometimes trigger detection updates repeatedly.
// To prevent keys from "flickering" or re-swapping, we add this.

#define OS_DETECTION_SINGLE_REPORT
#define OS_DETECTION_KEYBOARD_RESET
#define OS_DETECTION_DEBOUNCE 500

#define ONESHOT_TAP_TOGGLE 2  // Tapping this number of times holds the key until tapped once again.
#define CAPS_WORD_IDLE_TIMEOUT 30000  // 30 seconds

#define GAMING_LAYER_TOGGLE_THRESHOLD 1500 // 1.5 seconds required to toggle to gaming layer
