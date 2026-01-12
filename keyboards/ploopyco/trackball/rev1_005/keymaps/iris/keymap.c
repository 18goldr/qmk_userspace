#include QMK_KEYBOARD_H


#define DPI_1 1200
#define DPI_2 1600
#define DPI_3 1800
#define DPI_4 2000


// Define custom keycodes
enum custom_keycodes {
    CLK_SIG = SAFE_RANGE, // Left Click + Caps Lock Signal
    DPI_CYCLE             // Cycles through the 3 DPI levels
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        CLK_SIG,    MS_BTN3,    DPI_CYCLE,    MS_BTN2,    KC_NO
    ),
};

// Variables to track DPI state
static uint8_t dpi_level = 3;

// Set the default DPI when the trackball turns on
void keyboard_post_init_user(void) {
    // Updated for new PMW33XX driver (Sensor index 0, CPI value)
    pmw33xx_set_cpi(0, DPI_1);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CLK_SIG:
            if (record->event.pressed) {
                // 1. SIGNAL FIRST: Toggle Scroll Lock to tell Iris "Drop Shift!"
                // We blindly tap it. If it was off, it goes on. If on, it goes off.
                // The Iris just looks for *any* change or specific state.

                if (!host_keyboard_led_state().scroll_lock) {
                    tap_code(KC_SCRL);
                }

                // 2. WAIT: Give the Iris and OS time to process the signal (50ms)
                // This ensures Shift is gone BEFORE we click.
                wait_ms(7);

                // 3. CLICK: Now we click. Since Shift is gone, this click
                // will simply place the cursor and clear the blue highlight.
                register_code(MS_BTN1);


            } else {
                // Release logic
                unregister_code(MS_BTN1);

                // Clean up the light (Turn it off if we left it on)
                if (host_keyboard_led_state().scroll_lock) {
                    tap_code(KC_SCRL);
                }
            }
            return false;

        case DPI_CYCLE:
            if (record->event.pressed) {
                dpi_level = (dpi_level + 1) % 4;
                switch (dpi_level) {
                    case 0: pmw33xx_set_cpi(0, DPI_1); break;
                    case 1: pmw33xx_set_cpi(0, DPI_2); break;
                    case 2: pmw33xx_set_cpi(0, DPI_3); break;
                    case 3: pmw33xx_set_cpi(0, DPI_4); break;
                }
            }
            return false;
    }
    return true;
}