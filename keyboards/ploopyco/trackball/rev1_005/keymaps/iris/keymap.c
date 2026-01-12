#include QMK_KEYBOARD_H

#define NUM_DPI 5

// Create an array for easy lookup
static uint16_t dpi_values[NUM_DPI] = {1200, 1400, 1600, 1700, 1800};


// Define custom keycodes
enum custom_keycodes {
    CLK_SIG = SAFE_RANGE, // Left Click + Scroll Lock Signal
    DPI_CYCLE             // Cycles through the DPI levels
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        CLK_SIG,    MS_BTN3,    DPI_CYCLE,    MS_BTN2,    KC_NO
    ),
};

// Variables to track DPI state
static uint8_t dpi_level = 3;


#ifdef OS_DETECTION_ENABLE
bool process_detected_host_os_user(os_variant_t detected_os) {
    uprintf("detected host OS user: %d\n", detected_os);
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
			dpi_values[0] = 1300;
			dpi_values[1] = 1400;
			dpi_values[2] = 1500;
			dpi_values[3] = 1600;
			dpi_values[4] = 1700;
            dpi_level = 0;
            break;
        default:
            break;
    }
    return true;
}
#endif



void keyboard_post_init_user(void) {
    uint16_t initial_dpi = dpi_values[dpi_level];

    uprintf("Post init: Level %d (Value %d)\n", dpi_level, initial_dpi);
    pmw33xx_set_cpi(0, initial_dpi);
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
                dpi_level = (dpi_level + 1) % NUM_DPI;
                uint16_t new_dpi = dpi_values[dpi_level];

                uprintf("Cycled to level: %d, PI: %d\n", dpi_level, new_dpi);
                pmw33xx_set_cpi(0, new_dpi);
            }
            return false;
    }
    return true;
}
