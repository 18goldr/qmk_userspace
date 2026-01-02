#include QMK_KEYBOARD_H
#include "os_detection.h" // Needed for debug and stability
#include <keymap_introspection.h>

#include "layers.h"
#include "state_config.h"
#include "select_region.h"

static void kill_line(void) {
    // If your selection mode is on, turn it off so Shift doesn't affect things
    if (selecting) {
        selection_set(false);
    }

    // Select to end-of-line (OS-specific)
    if (host_os == OS_MACOS || host_os == OS_IOS) {
        // Cmd+Shift+Right selects to end of line on macOS in most apps
        // Since we swap control and gui automatically, then we send a keypress for gui instead of command here
        tap_code16(S(G(KC_RGHT)));
    } else {
        // Shift+End selects to end of line on Windows/Linux in most apps
        tap_code16(S(KC_END));
    }

    // Delete selection (if already at EOL, this typically deletes the newline/next char)
    tap_code(KC_DEL);
}

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
            return TAPPING_TERM * 2;  // I don't use alt or gui that often
        default:
          return TAPPING_TERM;
  }
}

// Custom Keycode Handler: Manages Layer switching and the persistent Alt-Tab macro.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Ctrl+K => kill line (works with MT mods and doesn't "lose" Ctrl afterward)
    if (record->event.pressed && ctrl_is_down()) {
        uint16_t tap_kc = get_tap_keycode(keycode);
        if (tap_kc == KC_K) {

            // Save current modifier state (this is CRITICAL for home-row mods)
            const uint8_t saved_mods     = get_mods();
            const uint8_t saved_oneshot  = get_oneshot_mods();
            const uint8_t saved_weak     = get_weak_mods();

            // Temporarily remove Ctrl so our selection keystrokes aren't modified by Ctrl
            del_mods(MOD_MASK_CTRL);
            del_oneshot_mods(MOD_MASK_CTRL);
            del_weak_mods(MOD_MASK_CTRL);

            kill_line();

            // Restore modifier state so Ctrl still works while you're holding it
            set_mods(saved_mods);
            set_oneshot_mods(saved_oneshot);
            set_weak_mods(saved_weak);

            return false; // swallow the K tap so the OS never sees Ctrl+K
        }
    }

    switch (keycode) {
        // Persistent Layer Changes
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;

        case LT(_NAV, KC_SPC): // selection mode
            if (record->event.pressed) {
                ctrl_space_armed = ctrl_is_down();
                return true; // let LT logic proceed (tap vs hold decided later)
            } else {
                if (ctrl_space_armed && record->tap.count > 0) {
                    // Prevent OS/app from seeing Ctrl+Space
                    del_mods(MOD_MASK_CTRL);
                    del_oneshot_mods(MOD_MASK_CTRL);

                    selection_set(!selecting);

                    ctrl_space_armed = false;
                    return false; // swallow the tap so Space doesn't emit
                }
                ctrl_space_armed = false;
                return true;
            }

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

    // ---- Emacs-y: if selection mode is on, typing cancels it ----
    if (selecting) {
        uint16_t tap_kc = get_tap_keycode(keycode);

        // Never cancel selection for navigation keys or pure modifiers
        if (!is_selection_nav(tap_kc) && !IS_MODIFIER_KEYCODE(keycode)) {

            // IMPORTANT FIX:
            // For Mod-Tap / Layer-Tap typing keys, cancel on PRESS so Shift is released
            // BEFORE QMK decides/sends the tap character.
            if (
                record->event.pressed &&
                (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) &&
                is_typing_key(tap_kc)
            ) {
                selection_set(false);
            }

            // Regular keys: cancel on press if it's a typing key
            if (
                record->event.pressed &&
                !(IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) &&
                is_typing_key(keycode)
            ) {
                selection_set(false);
            }
        }
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
    host_os = detected_os;

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