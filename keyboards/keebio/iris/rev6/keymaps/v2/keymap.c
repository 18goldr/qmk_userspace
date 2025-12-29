#include QMK_KEYBOARD_H
#include "os_detection.h" // Needed for debug and stability
#include <keymap_introspection.h>

#include "keycode_definitions.h"
#include "layers.h"
#include "state_config.h"


// Custom Tapping Term: Adjusts how long you must hold a key for it to become a modifier.
// Improves Home Row Mod accuracy by making index fingers faster and pinkies more deliberate.
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

// Custom Keycode Handler: Manages Layer switching and the persistent Alt-Tab macro.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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


// RGB Indicators: Updates the LEDs based on keyboard state.
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
        case LSFT_T(KC_F):
            // Immediately select the hold action when another key is pressed.
            return true;
		case RSFT_T(KC_J):
			return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record,
                           uint16_t prev_keycode) {
	uint16_t tap_code = get_tap_keycode(keycode);
	uint16_t prev_tap_code = get_tap_keycode(prev_keycode);
	// INDEX SHIFT PROTECTION (F and J)
	if (prev_tap_code == KC_F) {
		switch (tap_code) {
			case KC_SLSH:
			case KC_SCLN:
			case KC_ENT:
			case KC_QUOT:
			case KC_I:
				return 0;  // I rarely want to type f/ or f;, f'. I also rarely do f then click enter. I also want to prioritize "I" over "fi"
				break;
			default:
				// Pattern: fa, fl, fj, fr, just, join
				return FLOW_TAP_TERM;
				break;
		}
	// I dont really use right shift when typing fast.
	} else if (prev_tap_code == KC_J) {
		return FLOW_TAP_TERM;

	// THUMB LAYER PROTECTION (Space)
	} else if (prev_tap_code == KC_SPC) {
		switch (tap_code) {
			case KC_E:
			case KC_D:
			case KC_F:
			case KC_S:
				return FLOW_TAP_TERM / 2;  // I could actually want a letter after a space.
				break;
			default:
				return FLOW_TAP_TERM;
				break;
		}
	}
	// CONTROL PROTECTION (D)
	else if (prev_tap_code == KC_D) {
		switch (tap_code) {
			case KC_Z:
			case KC_S:
			case KC_X:
			case KC_C:
			case KC_V:
				return 0; // dz, dx, dc, dv, ds are things I rarely type (unless I'm doing calculus).
				break;
			case KC_R:
				return FLOW_TAP_TERM / 2; // Protects words like drill, dry, etc. But still allows for control + r in the temrminal
				break;
			default:
				// If it's not a shortcut or 'r', we're likely mashing or typing a rare combo.
                // We'll give it a standard term to be safe.
				// Pattern matched: da
				return FLOW_TAP_TERM;
				break;
		}
	}
	// WEAK FINGER PROTECTION (A, S, P, L)
    // We only apply FLOW_TAP_TERM to specific valid english patterns
    switch (prev_tap_code) {
        case KC_A:
            switch (tap_code) {
                case KC_L:
				case KC_S:
				case KC_D:
				case KC_F:
				case KC_G:
				case KC_P:
                    return FLOW_TAP_TERM; // al, as, ad, af, ag, ap
					break;
            }
            break;

        case KC_S:
            switch (tap_code) {
                case KC_A:
				case KC_H:
				case KC_K:
				case KC_L:
                    return FLOW_TAP_TERM; // sa, sh, sk, sl
					break;
            }
            break;

        case KC_P:
        case KC_L:
            if (tap_code == KC_A)
			{
				return FLOW_TAP_TERM; // pa, la
			}
			break;
    }

    return 0;  // Disable Flow Tap.
}


bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {

    // We use your existing logic to see if this is a "flow" (typing) or "shortcut"
    // Note: get_flow_tap_term takes the raw keycodes, which is perfect here.
    uint16_t flow_term = get_flow_tap_term(other_keycode, other_record, tap_hold_keycode);

    // If Flow Tap returns 0, it means:
    // 1. It's an index shift exception (like KC_I)
    // 2. It's a Ctrl shortcut (like KC_Z, KC_X, KC_C)
    // 3. It's a "nonsense" mash that isn't a valid English roll.
    if (flow_term == 0) {
        return true; // Allow the HOLD (Modifier) to trigger instantly
    }

    // For everything else, defer to the default "Opposite Hands" rule.
    // This will force "face" (same hand) to be a tap, and "I" (opposite hand) to be a hold.
    return get_chordal_hold_default(tap_hold_record, other_record);
}
