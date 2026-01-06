#include QMK_KEYBOARD_H

static bool is_alt_tab_active = false;
static uint16_t alt_tab_timer = 0;

// QMK automatically calls this because the function name matches the folder name 'alt_tab'
bool process_record_alt_tab(uint16_t keycode, keyrecord_t *record) {

    // Check for the keycodes we defined in qmk_module.json
    if (keycode == ALT_TAB_NEXT || keycode == ALT_TAB_PREV) {
        if (record->event.pressed) {
            // Activate Alt-Tab interface if not already active
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }

            // Reset the auto-close timer
            alt_tab_timer = timer_read();

            // Send Tab or Shift+Tab
            if (keycode == ALT_TAB_NEXT) {
                register_code(KC_TAB);
            } else {
                register_code16(S(KC_TAB));
            }
        } else {
            // Release Tab on key release
            unregister_code(KC_TAB);
            unregister_code16(S(KC_TAB));
        }
        return false; // Swallow the key so the OS doesn't see "ALT_TAB_NEXT"
    }

    return true; // Let other keys pass through
}

// Runs every loop to check the timer
void housekeeping_task_alt_tab(void) {
    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 600) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
}