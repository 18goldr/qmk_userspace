#include QMK_KEYBOARD_H
#include "os_detection.h" // Needed for debug and stability
#include <keymap_introspection.h>

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

/*
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
*/


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    uint16_t tap_code = get_tap_keycode(keycode);
    switch (tap_code) {
        // IMPORTANT:
        // Do NOT include KC_F here. You want Shift-hold only for specific chords (I / ; / ' / / / Enter),
        // and HOO cannot be conditional on which other key was pressed.
        case KC_J:
            return true;   // keep if you like it for right shift; remove if it causes similar issues
        default:
            return false;
    }
}


uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record,
                           uint16_t prev_keycode) {
    uint16_t tap_code      = get_tap_keycode(keycode);
    uint16_t prev_tap_code = get_tap_keycode(prev_keycode);

    // INDEX SHIFT PROTECTION (F and J)
    if (prev_tap_code == KC_F) {
        switch (tap_code) {
            case KC_SCLN:
            case KC_QUOT:
            case KC_I:
            case KC_SLSH:  // add back: for '?' (Shift + /)
            case KC_ENT:   // add back: for Shift+Enter (esp. with LT(_SYM, KC_ENT))
                return 0;  // prioritize HOLD (Shift) for these chords
            default:
                return FLOW_TAP_TERM; // allow normal "flow" typing like fl/fr/fa...
        }
    } else if (prev_tap_code == KC_J) {
        return FLOW_TAP_TERM;

    // THUMB LAYER PROTECTION (Space)
    } else if (prev_tap_code == KC_SPC) {

        // IMPORTANT:
        // Don't apply the "space -> short flow term" behavior to your LSFT_T(KC_F),
        // otherwise Space->F->I can commit 'f' before Shift engages.
        if (keycode == LSFT_T(KC_F)) {
            return 0;
        }

        switch (tap_code) {
            case KC_E:
            case KC_D:
            case KC_F:
            case KC_S:
                return FLOW_TAP_TERM / 2;
            default:
                return FLOW_TAP_TERM;
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
                return 0; // prefer Ctrl shortcuts
            case KC_R:
                return FLOW_TAP_TERM / 2; // allow Ctrl+R but keep drill/dry protection
            default:
                return FLOW_TAP_TERM;
        }
    }

    // WEAK FINGER PROTECTION (A, S, P, L)
    switch (prev_tap_code) {
        case KC_A:
            switch (tap_code) {
                case KC_L:
                case KC_S:
                case KC_D:
                case KC_F:
                case KC_G:
                case KC_P:
                    return FLOW_TAP_TERM;
            }
            break;

        case KC_S:
            switch (tap_code) {
                case KC_A:
                case KC_H:
                case KC_K:
                case KC_L:
                    return FLOW_TAP_TERM;
            }
            break;

        case KC_P:
        case KC_L:
            if (tap_code == KC_A) {
                return FLOW_TAP_TERM;
            }
            break;
    }

    return 0;
}


bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {
    uint16_t other_tap = get_tap_keycode(other_keycode);

    // 1) Left Shift home-row mod: HOLD only for specific "I / : / \" / ? / Shift+Enter" intents
    //    This prevents "flow" -> "Low" while still making your most common Shift chords instant.
    if (tap_hold_keycode == LSFT_T(KC_F)) {
        switch (other_tap) {
            case KC_I:
            case KC_SCLN: // ':' when shifted
            case KC_QUOT: // '"' when shifted
            case KC_SLSH: // '?' when shifted
            case KC_ENT:  // Shift+Enter (including LT(_SYM, KC_ENT) because tap keycode is KC_ENT)
                return true;   // force HOLD -> Shift
            default:
                return false;  // otherwise force TAP -> keep 'f' for normal words like "flow"
        }
    }

    // 2) Ctrl shortcuts: make LCTL_T(KC_D) HOLD immediately for common Ctrl combos
    if (tap_hold_keycode == LCTL_T(KC_D)) {
        switch (other_tap) {
            case KC_Z:
            case KC_S:
            case KC_X:
            case KC_C:
            case KC_V:
            case KC_R:
                return true;   // force HOLD -> Ctrl
            default:
                break;         // otherwise fall through to your flow logic
        }
    }

    // 3) NAV layer: make LT(_NAV, KC_SPC) HOLD immediately for NAV cluster keys
    if (tap_hold_keycode == LT(_NAV, KC_SPC)) {
        switch (other_tap) {
            case KC_S:
            case KC_D:
            case KC_F:
            case KC_E:
                return true;   // force HOLD -> NAV layer
            default:
                break;
        }
    }

    // Existing logic for everything else (your flow-based decision making)
    uint16_t flow_term = get_flow_tap_term(other_keycode, other_record, tap_hold_keycode);

    if (flow_term == 0) {
        return true; // allow HOLD (modifier/layer) to trigger instantly
    }

    return get_chordal_hold_default(tap_hold_record, other_record);
}
