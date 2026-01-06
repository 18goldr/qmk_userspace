#pragma once
#include QMK_KEYBOARD_H

extern bool selecting;

bool ctrl_is_down(void);
void selection_set(bool on);