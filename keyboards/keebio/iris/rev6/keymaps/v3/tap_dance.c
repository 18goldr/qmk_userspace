#include QMK_KEYBOARD_H
#include "tap_dance.h"

void tap_dance_caps_word_caps_lock(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Single Tap: Toggle Caps Word or disable caps lock if it's enabled
        if (host_keyboard_led_state().caps_lock) {
            tap_code(KC_CAPS);
        }
        else {
            caps_word_toggle();
        }
    } else if (state->count == 2) {
        // Double Tap: Trigger standard Caps Lock
        tap_code(KC_CAPS);
    }
}
