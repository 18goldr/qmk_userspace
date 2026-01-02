#pragma once

// Tap Dance declarations
enum {
    CW_CL,
};

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

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Cap Words, twice for Caps Lock
    [CW_CL] = ACTION_TAP_DANCE_FN(tap_dance_caps_word_caps_lock),
};

