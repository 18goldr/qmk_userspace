#include "select_region.h"
#include "helpers.h"
#include "led.h"
#include "print.h"
sa
static bool primary_space_armed = false;


static bool is_typing_key(uint16_t kc) {
    if (kc >= KC_A && kc <= KC_Z) return true;
    if (kc >= KC_1 && kc <= KC_0) return true;
    switch (kc) {
        case KC_SPC: case KC_TAB: case KC_ENT: case KC_BSPC: case KC_DEL:
        case KC_MINS: case KC_EQL: case KC_LBRC: case KC_RBRC:
        case KC_BSLS: case KC_SCLN: case KC_QUOT: case KC_COMM:
        case KC_DOT: case KC_SLSH: case KC_GRV:
            return true;
        default: return false;
    }
}

static bool is_selection_nav(uint16_t kc) {
    switch (kc) {
        case KC_LEFT: case KC_RGHT: case KC_UP: case KC_DOWN:
        case KC_HOME: case KC_END: case KC_PGUP: case KC_PGDN:
        case LCTL(KC_RIGHT): case LCTL(KC_LEFT):
            return true;
        default: return false;
    }
}

// The main processing function for this feature
bool process_select_region(uint16_t keycode, keyrecord_t *record) {
    uint16_t tap_kc = get_tap_keycode(keycode);

    // Handle Ctrl+Space
    if (tap_kc == KC_SPC) {
        if (record->event.pressed) {
            primary_space_armed = primary_mod_is_down();
            return true;
        } else {
            if (primary_space_armed && record->tap.count > 0) {
                selecting = !selecting;
                primary_space_armed = false;
                return false; // Swallow the release
            }
            primary_space_armed = false;
            return true;
        }
    }

    // Handle Cancellation (ESC or Typing)
    if (selecting && record->event.pressed) {
        // ESC cancels
        if (tap_kc == KC_ESC) {
            selecting = false;
        }
        // Typing cancels
        else if (!is_selection_nav(tap_kc) && !IS_MODIFIER_KEYCODE(keycode)) {
            bool is_mt_typing = (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && is_typing_key(tap_kc);
            bool is_reg_typing = !(IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && is_typing_key(keycode);

            if (is_mt_typing || is_reg_typing) {
                selecting = false;
            }
        }
        else if (is_selection_nav(tap_kc)) {

            // Word-jump nav (Ctrl+Arrow)
            if (keycode == LCTL(KC_LEFT) || keycode == LCTL(KC_RIGHT)) {
                tap_code16(S(keycode));   // Shift + Ctrl + Arrow
            }
            // Plain nav
            else {
                tap_code16(S(tap_kc));    // Shift + Arrow/Home/End/etc
            }

            return false;
        }
    }

    return true; // Always allow the key to pass through (except Ctrl+Space above)
}