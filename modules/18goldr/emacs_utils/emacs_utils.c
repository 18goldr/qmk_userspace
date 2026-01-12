#include QMK_KEYBOARD_H
#include "os_detection.h"
#include "kill_line.h"
#include "select_region.h"
#include "helpers.h"

bool process_detected_host_os_emacs_utils(os_variant_t detected_os) {
    module_os = detected_os;
    return true;
}

bool process_record_emacs_utils(uint16_t keycode, keyrecord_t *record) {
#ifdef SELECT_REGION_ENABLE
    // If it returns false (it handled Ctrl+Space), we stop.
    if (!process_select_region(keycode, record)) {
        return false;
    }
#endif

#ifdef KILL_LINE_ENABLE
    // If it returns false (it handled Ctrl+K), we stop.
    if (!process_kill_line(keycode, record, module_os)) {
        return false;
    }
#endif
    return true;
}