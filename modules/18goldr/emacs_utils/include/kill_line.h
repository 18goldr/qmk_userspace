#pragma once
#include "quantum.h"
#include "os_detection.h"

#ifdef KILL_LINE_ENABLE

bool process_kill_line(uint16_t keycode, keyrecord_t *record, os_variant_t current_os);
void kill_line(os_variant_t current_os);

#endif