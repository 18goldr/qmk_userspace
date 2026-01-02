#include QMK_KEYBOARD_H
#include "os_detection.h" // Needed for debug and stability
#include <keymap_introspection.h>

#include "layers.h"
#include "state_config.h"


// Custom Tapping Term: Adjusts how long you must hold a key for it to become a modifier.
// Improves Home Row Mod accuracy by making index fingers faster and pinkies more deliberate.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RCTL_T(KC_K):
        case RALT_T(KC_L):
        case RGUI_T(KC_P):
        case LALT_T(KC_S):
        case LGUI_T(KC_A):
            return TAPPING_TERM * 2;  // I don't use alt or gui that often
        default:
          return TAPPING_TERM;
  }
}

// Custom Keycode Handler: Manages Layer switching and the persistent Alt-Tab macro.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	uprintf("ID: %d | P: %u | T: %u\n", keycode, record->event.pressed, record->event.time);
  switch (keycode) {
    // Persistent Layer Changes
    case QWERTY:
      if (record->event.pressed) {
          set_single_persistent_default_layer(_QWERTY);
      }
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

	case TOGG_GAME:
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

// Matrix Scan: Runs constantly. Used here to release the 'Alt' key
// if you haven't pressed the Alt-Tab keys for 600ms.
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

void caps_word_set_user(bool active) {
    if (active) {
        rgb_matrix_set_color(25, RGB_RED);
        rgb_matrix_set_color(12, RGB_RED);
    } else {
        rgb_matrix_set_color(25, colourDefault[0], colourDefault[1], colourDefault[2]);
        rgb_matrix_set_color(12, colourDefault[0], colourDefault[1], colourDefault[2]);
    }
}

// RGB Indicators: Updates the LEDs based on keyboard layer.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
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