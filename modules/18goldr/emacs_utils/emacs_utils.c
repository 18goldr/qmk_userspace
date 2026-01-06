#include QMK_KEYBOARD_H
#include "os_detection.h"
#include "kill_line.h"
#include "select_region.h"

static os_variant_t module_os = OS_UNSURE;

bool process_detected_host_os_emacs_utils(os_variant_t detected_os) {
    module_os = detected_os;
    return true;
}

bool process_record_emacs_utils(uint16_t keycode, keyrecord_t *record) {
#ifdef SELECT_REGION_ENABLE
    // Check Select Region Feature
    // If it returns false (it handled Ctrl+Space), we stop.
    if (!process_select_region(keycode, record)) {
        return false;
    }
#endif

#ifdef KILL_LINE_ENABLE
    // Check Kill Line Feature
    // If it returns false (it handled Ctrl+K), we stop.
    if (!process_kill_line(keycode, record, module_os)) {
        return false;
    }
#endif

    // 3. Otherwise, let QMK proceed
    return true;
}

bool led_update_user(led_t led_state) {

    // Check our signal whenever LEDs change
#ifdef SELECT_REGION_ENABLE
    // You'll need to expose the function in select_region.h first!
    check_select_region_signal();
#endif

    return true;
}