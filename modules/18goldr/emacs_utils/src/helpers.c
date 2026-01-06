#include "helpers.h"

bool selecting = false;

bool ctrl_is_down(void) {
    // We check all modification states to be safe
    uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();
    return mods & MOD_MASK_CTRL;
}

void selection_set(bool on) {
    selecting = on;
    if (selecting) {
        register_code(KC_LSFT);     // Hold Shift
    } else {
        unregister_code(KC_LSFT);   // Release Shift
    }
}