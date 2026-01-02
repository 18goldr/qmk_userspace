#pragma once

// State trackers for selection region feature
static bool selecting = false;
static bool ctrl_space_armed = false;
//static uint16_t sel_last_nav = KC_RGHT; // default direction



static inline bool ctrl_is_down(void) {
    uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();
    return mods & MOD_MASK_CTRL;
}

static void selection_set(bool on) {
    selecting = on;
    if (selecting) {
        register_code(KC_LSFT);     // hold Shift (key event)
    } else {
        unregister_code(KC_LSFT);   // release Shift immediately
    }
}


// Treat these as "typing" (i.e., should cancel selection mode like Emacs)
static bool is_typing_key(uint16_t kc) {
    // letters
    if (kc >= KC_A && kc <= KC_Z) return true;
    // numbers
    if (kc >= KC_1 && kc <= KC_0) return true;

    switch (kc) {
        case KC_SPC:
        case KC_TAB:
        case KC_ENT:
        case KC_BSPC:
        case KC_DEL:

        // common punctuation/symbol keys
        case KC_MINS:
        case KC_EQL:
        case KC_LBRC:
        case KC_RBRC:
        case KC_BSLS:
        case KC_SCLN:
        case KC_QUOT:
        case KC_COMM:
        case KC_DOT:
        case KC_SLSH:
        case KC_GRV:
            return true;
        default:
            return false;
    }
}

// Movement keys that should NOT cancel selection
static bool is_selection_nav(uint16_t kc) {
    switch (kc) {
        case KC_LEFT: case KC_RGHT: case KC_UP: case KC_DOWN:
        case KC_HOME: case KC_END:
        case KC_PGUP: case KC_PGDN:
            return true;
        default:
            return false;
    }
}