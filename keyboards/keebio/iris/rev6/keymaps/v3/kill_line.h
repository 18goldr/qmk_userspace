#pragma once

static os_variant_t host_os = OS_UNSURE;

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