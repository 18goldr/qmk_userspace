#pragma once

// Enables the 'get_tapping_term' function in your keymap.c.
// This allows you to set different timings for different keys (like your home row mods).
#define TAPPING_TERM_PER_KEY

// The default time (in milliseconds) a key must be held to register as a modifier.
// At 180ms, this is a relatively "fast" setting, good for quick typists.
#define TAPPING_TERM 180

// Tells QMK to look at the EEPROM (on-board memory) to determine which half is left/right.
// This allows you to plug the USB cable into either side of the Iris.
#define EE_HANDS

// Ensures that both halves of the split keyboard stay in sync regarding which
// layer is currently active, so RGB indicators work correctly on both sides.
#define SPLIT_LAYER_STATE_ENABLE

// ARM-based Macs (M1/M2/M3) can sometimes trigger detection updates repeatedly.
// To prevent keys from "flickering" or re-swapping, we add this.