#ifndef QMK_USERSPACE_KEYCODE_DEFINITIONS_H
#define QMK_USERSPACE_KEYCODE_DEFINITIONS_H

// Home Row Mods: Hold for Modifier, Tap for Key (Left Hand)
#define GUI_A   MT(MOD_LGUI, KC_A)
#define ALT_S   LALT_T(KC_S)
#define CTL_D   MT(MOD_LCTL, KC_D)
#define SFT_F   LSFT_T(KC_F)

// Home Row Mods: Hold for Modifier, Tap for Key (Right Hand)
#define SFT_J RSFT_T(KC_J)
#define CTL_K MT(MOD_RCTL, KC_K)
#define ALT_L RALT_T(KC_L)
#define GUI_P MT(MOD_RGUI, KC_SCLN)

// Dual-function Thumb Keys: Hold for Layer, Tap for Key
#define ENT_SYM LT(_SYM, KC_ENT)
#define SPC_NAV LT(_NAV, KC_SPC)

// Custom keycodes for unique macros and layer toggles
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  SYMBOLS,
  NUM,
  NAV,
  GAMING,
  PREVWIN, // Custom Alt-Tab (Backward)
  NEXTWIN, // Custom Alt-Tab (Forward)
  LONG_TOGGLE_GAMING
};

#endif // QMK_USERSPACE_KEYCODE_DEFINITIONS_H


