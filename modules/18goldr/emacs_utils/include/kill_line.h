#pragma once
#include "quantum.h"
#include "os_detection.h"

// Returns false if the key was consumed (Ctrl+K), true otherwise
bool process_kill_line(uint16_t keycode, keyrecord_t *record, os_variant_t current_os);

// Helper function used internally by process_kill_line
void kill_line(os_variant_t current_os);