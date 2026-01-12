#include "helpers.h"
#include "os_detection.h"

bool selecting = false;

os_variant_t module_os = OS_UNSURE;

inline bool primary_mod_is_down(void) {
    uint8_t mods = get_mods() | get_oneshot_mods() | get_weak_mods();

    if (module_os == OS_MACOS || module_os == OS_IOS) {
        return mods & MOD_MASK_GUI;   // use GUI on macOS
    } else {
        return mods & MOD_MASK_CTRL;  // use CTRL on Windows/Linux
    }
}