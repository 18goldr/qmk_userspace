/*
Copyright 2017 Danny Nguyen <danny@keeb.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Sets how many times you must tap a one-shot key to make it "stick" (toggle).
// Useful for layer-toggle keys if you want to lock a layer on.
#define TAPPING_TOGGLE 2

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
#define OS_DETECTION_SINGLE_REPORT
#define OS_DETECTION_KEYBOARD_RESET
#define OS_DETECTION_DEBOUNCE 500


//#define CHORDAL_HOLD

/*
// See https://docs.qmk.fm/tap_hold for descriptions of these
#define QUICK_TAP_TERM_PER_KEY
#define PERMISSIVE_HOLD
// If a key is held then released alone, it sends the tap value regardless of time.
//#define RETRO_TAPPING
#define RETRO_TAPPING_PER_KEY

// Tap for the character, hold and release alone for the shifted version.
#define RETRO_SHIFT
// This ensures that holding the key past the TAPPING_TERM still results
// in a shifted character on release.
#define RETRO_SHIFT_HOLD_TYPE
*/