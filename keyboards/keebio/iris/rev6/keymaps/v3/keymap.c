#include QMK_KEYBOARD_H
#include "os_detection.h" // Needed for debug and stability
#include <keymap_introspection.h>

#include "layers.h"

// RGB Colors for layers and caps lock
uint8_t colourDefault[] = {57, 184, 255}; // Light Blue
uint8_t colourGaming[]  = {255, 0, 0};    // Red

// Custom Tapping Term: Adjusts how long you must hold a key for it to become a modifier.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOLD_TG_GAME:
            return GAMING_LAYER_TOGGLE_THRESHOLD;
        case RCTL_T(KC_K):
        case RALT_T(KC_L):
        case RGUI_T(KC_P):
        case LALT_T(KC_S):
        case LGUI_T(KC_A):
            return TAPPING_TERM * 2;
        default:
          	return TAPPING_TERM;
  	}
}

// Custom Keycode Handler: Manages Layer switching, persistent Alt-Tab macro
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Persistent Layer Changes
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;

        case HOLD_TG_GAME:
            if (record->event.pressed) {
                if (record->tap.count == 0) {
                    // This triggers if you've held the key past 1500ms
                    layer_invert(_GAMING);
                    sentence_case_toggle();  // Disable sentence case in gaming mode
                    return false;
                }
            }
            break;
    }
    return true;
}


#ifdef RGB_MATRIX_ENABLE
layer_state_t layer_state_set_user(layer_state_t state) {

    // Check which layer is active
    switch (get_highest_layer(state)) {
        case _GAMING:
            // Set ENTIRE keyboard to Gaming Color (Efficient!)
            // Format: (Hue, Saturation, Value)
            rgb_matrix_sethsv_noeeprom(0, 255, 100); // Red
            break;

        default:
            // Set ENTIRE keyboard back to Default Color
            rgb_matrix_sethsv_noeeprom(128, 255, 100); // Teal
            break;
    }

    return state;
}

void keyboard_post_init_user(void) {
    // Force the mode to Solid Color so it doesn't use the heavy "Indicator" logic
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);

    // Turn them ON to the default color immediately
    rgb_matrix_enable_noeeprom(); // Ensure the system is actually on
    rgb_matrix_sethsv_noeeprom(128, 255, 100); // Teal (Default)
}
#endif


#ifdef OS_DETECTION_ENABLE
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            keymap_config.swap_lctl_lgui = true;
            break;
        default:
            keymap_config.swap_lctl_lgui = false;
            break;
    }
    return true;
}
#endif