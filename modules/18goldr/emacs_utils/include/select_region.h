#pragma once
#include "quantum.h"

// Returns false if the key was consumed (Ctrl+Space), true otherwise
bool process_select_region(uint16_t keycode, keyrecord_t *record);