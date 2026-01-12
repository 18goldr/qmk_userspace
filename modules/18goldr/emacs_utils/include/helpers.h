#pragma once
#include QMK_KEYBOARD_H
#include "os_detection.h"

extern bool selecting;
extern os_variant_t module_os;

bool primary_mod_is_down(void);