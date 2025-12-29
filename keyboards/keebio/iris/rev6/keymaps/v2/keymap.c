#include QMK_KEYBOARD_H
#include "os_detection.h" // Needed for debug and stability
#include <keymap_introspection.h>

#include "keycode_definitions.h"
#include "layers.h"
#include "state_config.h"

/**
 * Custom Tapping Term: Adjusts how long you must hold a key for it to become a modifier.
 * Improves Home Row Mod accuracy by making index fingers faster and pinkies more deliberate.
 */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_F):
        case RSFT_T(KC_J):
            return TAPPING_TERM - 30; // Faster response for index fingers
        case LCTL_T(KC_D):
            return TAPPING_TERM - 15;
        case MT(MOD_LGUI, KC_A):
          return TAPPING_TERM + 70; // More delay for pinkies to prevent accidental triggers
        case MT(MOD_RCTL, KC_K):
        case MT(MOD_RGUI, KC_SCLN):
        case RALT_T(KC_L):
            return TAPPING_TERM * 2;  // I don't use right side mods that often
        default:
          return TAPPING_TERM;
  }
}

/**
 * Custom Keycode Handler: Manages Layer switching and the persistent Alt-Tab macro.
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Persistent Layer Changes
    case QWERTY:
      if (record->event.pressed) { set_single_persistent_default_layer(_QWERTY); }
      return false;

    // Alt-Tab Macros (Forward and Backward)
    case NEXTWIN:
    case PREVWIN:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT); // Hold Alt down
        }
        alt_tab_timer = timer_read(); // Reset auto-release timer
        if (keycode == NEXTWIN) {
            register_code(KC_TAB);
        } else {
            register_code16(S(KC_TAB)); // Shift+Tab for backward
        }
      } else {
        unregister_code(KC_TAB);
        unregister_code16(S(KC_TAB));
      }
      break;

	case LONG_TOGGLE_GAMING:
		if (record->event.pressed) {
        	toggle_key_pressed = true;
        	toggle_threshold_met = false; // Reset lock for a new press
        	toggle_hold_timer = timer_read();
    	} else {
       		toggle_key_pressed = false;
    	}
    	return false;
  }
  return true;
}

/**
 * Matrix Scan: Runs constantly. Used here to release the 'Alt' key
 * if you haven't pressed the Alt-Tab keys for 600ms.
 */
void matrix_scan_user(void) {
  	if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 600) {
    	unregister_code(KC_LALT);
    	is_alt_tab_active = false;
  	}

	if (toggle_key_pressed && !toggle_threshold_met) {
        if (timer_elapsed(toggle_hold_timer) > 1500) { // 1.5 second threshold
            layer_invert(_GAMING);   // Toggles the gaming layer
            toggle_threshold_met = true; // Lock the action so it doesn't toggle again
        }
    }
}

/**
 * RGB Indicators: Updates the LEDs based on keyboard state.
 */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  // Caps Lock Indicator
  if (host_keyboard_led_state().caps_lock) {
    rgb_matrix_set_color(25, RGB_RED);
    rgb_matrix_set_color(12, RGB_RED);
  }

  for (uint8_t i = led_min; i < led_max; i++) {
    // Layer Colors on Keys only
    switch(get_highest_layer(layer_state|default_layer_state)) {
      case _GAMING:
        rgb_matrix_set_color(i, colourGaming[0], colourGaming[1], colourGaming[2]);
        break;
      default:
        rgb_matrix_set_color(i, colourDefault[0], colourDefault[1], colourDefault[2]);
        break;
    }
  }
  return false;
}

// Callback function that runs automatically when the OS is detected
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


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_F:
        case SFT_J:
            // Immediately select the hold action when another key is pressed.
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}