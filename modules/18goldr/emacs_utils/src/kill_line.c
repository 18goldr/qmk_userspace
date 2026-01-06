#include "kill_line.h"
#include "helpers.h"

// The main processing function for this feature
bool process_kill_line(uint16_t keycode, keyrecord_t *record, os_variant_t current_os) {
    // Check for Ctrl + K
    if (record->event.pressed && ctrl_is_down()) {
        if (get_tap_keycode(keycode) == KC_K) {

            // Save state
            const uint8_t saved_mods = get_mods();
            const uint8_t saved_oneshot = get_oneshot_mods();
            const uint8_t saved_weak = get_weak_mods();

            // Clear Ctrl so it doesn't interfere
            del_mods(MOD_MASK_CTRL);
            del_oneshot_mods(MOD_MASK_CTRL);
            del_weak_mods(MOD_MASK_CTRL);

            // Execute the action
            kill_line(current_os);

            // Restore state
            set_mods(saved_mods);
            set_oneshot_mods(saved_oneshot);
            set_weak_mods(saved_weak);

            return false; // Swallow the 'K'
        }
    }
    return true; // Keep processing
}

// The actual action
void kill_line(os_variant_t current_os) {
    if (selecting) {
        selection_set(false);
    }
    if (current_os == OS_MACOS || current_os == OS_IOS) {
        tap_code16(S(G(KC_RGHT)));
    } else {
        tap_code16(S(KC_END));
    }
    tap_code(KC_DEL);
}