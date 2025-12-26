#include QMK_KEYBOARD_H

// ==========================================================================
// 1. LAYER DEFINITIONS & SHORTCUTS
// ==========================================================================
#define _QWERTY 0
#define _SYM    1
#define _FN     2
#define _NAV    3
#define _GAMING 4

// Home Row Mods: Hold for Modifier, Tap for Key (Left Hand)
#define GUI_A   MT(MOD_LGUI, KC_A)
#define ALT_S   MT(MOD_LALT, KC_S)
#define CTL_D   MT(MOD_LCTL, KC_D)
#define SFT_F   MT(MOD_LSFT, KC_F)

// Home Row Mods: Hold for Modifier, Tap for Key (Right Hand)
#define SFT_J   MT(MOD_RGUI, KC_J)
#define CTL_K   MT(MOD_RALT, KC_K)
#define ALT_L   MT(MOD_RCTL, KC_L)
#define GUI_SCLN MT(MOD_RSFT, KC_SCLN)

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
  REDO
};

// ==========================================================================
// 2. STATE VARIABLES & LIGHTING CONFIG
// ==========================================================================
bool is_alt_tab_active = false;  // Tracks if the Alt-Tab overlay is currently open
uint16_t alt_tab_timer = 0;     // Timer to auto-release Alt after a timeout

// RGB Colors for layers and caps lock
uint8_t colourDefault[] = {57, 184, 255}; // Light Blue
uint8_t colourGaming[]  = {255, 0, 0};    // Red

// LED Index Mapping (Iris Rev 6 specific)
int underglowLED[6] = {28, 29, 30, 31, 32, 33};
int backlightLEDinner[15] = {6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
int backlightLEDall[28] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

// ==========================================================================
// 3. KEYMAPS (Layers)
// ==========================================================================

  [_QWERTY] = LAYOUT(
  //┌────────┬────────────────────┬───────────────────┬───────────────────┬───────────────────┬────────────┐                              ┌───────────┬───────────────────────┬───────────────────┬───────────────────┬───────────────────────┬────────┐
     KC_GRV,    KC_1,                     KC_2,                KC_3,            KC_4,              KC_5,                                          KC_6,            KC_7,               KC_8,                KC_9,               KC_0,             KC_ESC,
  //├────────┼────────────────────┼───────────────────┼───────────────────┼───────────────────┼────────────┤                              ├───────────┼───────────────────────┼───────────────────┼───────────────────┼───────────────────────┼────────┼
     KC_LBRC,   KC_Q,                     KC_W,              KC_E,            KC_R,              KC_T,                                          KC_Y,            KC_U,                KC_I,                KC_O,               KC_SCLN,          KC_QUOT,
  //├────────┼────────────────────┼───────────────────┼───────────────────┼───────────────────┼────────────┤                              ├───────────┼───────────────────────┼───────────────────┼───────────────────┼───────────────────────┼────────┼
     KC_LCBR,   MT(MOD_LGUI, KC_A), MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_D), MT(MOD_LSFT, KC_F),  KC_G,                                          KC_H,      MT(MOD_RSFT, KC_J),  MT(MOD_RCTL, KC_K), MT(MOD_LALT, KC_L), MT(MOD_RGUI, KC_P),    KC_BSPC,
  //├────────┼────────────────────┼───────────────────┼───────────────────┼───────────────────┼────────────┼─────────┐           ┌────────┴───────────┼───────────────────────┼───────────────────┼───────────────────┼───────────────────────┼────────┼
     KC_TAB,   KC_Z,                   KC_X,                 KC_C,                KC_V,          KC_B,      MO(_FN),              TG(_GAMING),  KC_N,          KC_M,               KC_COMM,              KC_DOT,              KC_SLSH,          KC_BSLS,
  //└────────┴────────────────────┴───────────────────┴───────────────────┬───────────────────┴────────────┴─────────┘           └────────┴───────────┴───────────────────────┴───────────────────┴───────────────────┴───────────────────────┴────────┘
                                                                             PREVWIN,      ENT_SYM,         ENT_SYM,            SPC_NAV,     SPC_NAV,   NEXTWIN
                                               //                         └──────────────┴─────────────────┴─────────┘           └────────┴────────────┴──────────┘
  ),

  [_SYM] = LAYOUT(
    //┌─────────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
        _______,      _______, _______, _______, _______, KC_LBRC,                            KC_RBRC,   _______, _______, _______, _______, _______,
    //├─────────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
       _______,      _______, _______, _______, _______, KC_LPRN,                            KC_RPRN, _______, _______, _______ ,_______ , _______,
    //├─────────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
       KC_CAPS,      KC_MINS, KC_PLUS,KC_UNDS, KC_EQL,  KC_LCBR,                            KC_RCBR, _______, _______, _______, _______ , _______,
    //├─────────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
       _______,      _______, _______, _______, _______, KC_LT, _______,          _______,  KC_GT, _______, _______, _______, _______, _______,
    //└─────────────┴────────┴────────┴────────┴───┬────────────┼─────────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                        _______,     _______,     _______,              _______, _______, _______
                                   // └────────────┴────────────┴─────────┘            └────────┴────────┴────────┘
  ),

  [_FN] = LAYOUT(
    //┌──────────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
        KC_F12,       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,    KC_F8,  KC_F9,   KC_F10,  KC_F11,
    //├──────────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
        _______,       _______, _______,_______, _______, _______,                             _______, _______, _______, _______, _______,  KC_F12,
    //├──────────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
        _______,       _______, _______,_______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    //├──────────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
        _______,       _______, _______,_______, _______,  _______, _______,          _______,  _______, _______, _______, _______, _______, _______,
    //└──────────────┴────────┴────────┴────────┴───┬────────────┼─────────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                        _______,       _______,    _______,              _______, _______, _______
                                    // └────────────┴────────────┴─────────┘            └────────┴────────┴────────┘
  ),


  [_NAV] = LAYOUT(
  //┌─────────────┬─────────────┬────────┬────────┬────────┬─────────────┐                          ┌────────────┬─────────┬────────┬──────────┬───────────────┬────────┐
     _______,       _______,      _______, _______, _______, _______,                                  _______,    _______, _______, _______,     _______,      _______,
  //├─────────────┼─────────────┼────────┼────────┼────────┼─────────────┤                          ├────────────┼─────────┼────────┼──────────┼───────────────┼────────┤
     _______,      _______,       _______,  KC_UP,  _______, _______,                                  _______,    _______, KC_PGUP,  _______,     _______,     _______,
  //├─────────────┼─────────────┼────────┼────────┼────────┼─────────────┤                          ├────────────┼─────────┼────────┼──────────┼───────────────┼────────┤
     _______,      LCTL(KC_LEFT),KC_LEFT, KC_DOWN, KC_RIGHT,LCTL(KC_RIGHT),                           _______,    KC_HOME,  KC_PGDN, KC_END,     _______,       _______,
  //├─────────────┼─────────────┼────────┼────────┼────────┼─────────────┼────────┐        ┌────────┼────────────┼─────────┼────────┼──────────┼───────────────┼────────┤
     _______,        _______,    _______, _______, _______, _______,      _______,          _______,  _______,    _______,  _______, _______,    _______,       _______,
  //└─────────────┴─────────────┴────────┴┬───────┴───┬────┴───────┬─────┴────┬───┘        └───┬─────────────────┴─────────┴────────┴──────────┴───────────────┴────────┴
                                             _______,   _______,    _______,                     _______,          _______,       _______
                                      //  └───────────┴────────────┴──────────┘                 └─────────────┴──────────────┴────────────┘
  ),

  [_GAMING] = LAYOUT(
    //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────────┐
       _______,  _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______,  _______,
    //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────────┤
       KC_LALT, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────────┤
       KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    _______,                            _______,   KC_J,    KC_K,    KC_L,  KC_P,     _______,
    //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────────┤
       KC_LCTL, _______, _______, _______, _______, _______, TG(_GAMING),    TG(_GAMING),_______, _______, _______, _______, _______, _______,
    //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────────┘
                                      _______,  KC_SPC,  KC_ENT,                    _______, _______,  _______
    //                               └────────┴────────┴────────┘                 └────────┴────────┴────────┘
    ),
};


// ==========================================================================
// 4. CORE LOGIC & CUSTOM BEHAVIOR
// ==========================================================================

/**
 * Custom Tapping Term: Adjusts how long you must hold a key for it to become a modifier.
 * Improves Home Row Mod accuracy by making index fingers faster and pinkies more deliberate.
 */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case SFT_F:
    case SFT_J:
      return TAPPING_TERM - 30; // Faster response for index fingers
    case GUI_SCLN:
    case GUI_A:
      return TAPPING_TERM + 70; // More delay for pinkies to prevent accidental triggers
    default:
      return TAPPING_TERM;
  }
}

/**
 * Custom Keycode Handler: Manages Layer switching and the persistent Alt-Tab macro.
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // Persistent Layer Changes
    case QWERTY:
      if (record->event.pressed) { set_single_persistent_default_layer(_QWERTY); }
      return false;

    // Alt-Tab Macros (Forward and Backward)
    case NEXTWIN:
    case PREVWIN:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT); // Hold Alt down
        }
        alt_tab_timer = timer_read(); // Reset auto-release timer
        if (keycode == NEXTWIN) {
            register_code(KC_TAB);
        } else {
            register_code16(S(KC_TAB)); // Shift+Tab for backward
        }
      } else {
        unregister_code(KC_TAB);
        unregister_code16(S(KC_TAB));
      }
      break;
  }
  return true;
}

/**
 * Matrix Scan: Runs constantly. Used here to release the 'Alt' key
 * if you haven't pressed the Alt-Tab keys for 600ms.
 */
void matrix_scan_user(void) {
  if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 600) {
    unregister_code(KC_LALT);
    is_alt_tab_active = false;
  }
}

/**
 * RGB Indicators: Updates the LEDs based on keyboard state.
 */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // 1. Caps Lock Indicator (Keys 25 and 12 turn Red)
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(25, RGB_RED);
        rgb_matrix_set_color(12, RGB_RED);
    }

    // 2. Global Layer Colors (Sets the theme for the entire board)
    for (uint8_t i = led_min; i < led_max; i++) {
        switch(get_highest_layer(layer_state|default_layer_state)) {
            case _GAMING:
                rgb_matrix_set_color(i, colourGaming[0], colourGaming[1], colourGaming[2]);
                break;
            default:
                rgb_matrix_set_color(i, colourDefault[0], colourDefault[1], colourDefault[2]);
                break;
        }
    }
    return false;
}