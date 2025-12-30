# Custom Build Options

# Disables the full Bootmagic menu to save space; uses a simpler "Lite" version instead.
BOOTMAGIC_ENABLE = no

# Disables the ability to control the mouse cursor via keyboard keys.
MOUSEKEY_ENABLE = no

# Disables media keys (Volume, Play/Pause) and System keys (Power, Sleep).
EXTRAKEY_ENABLE = no

# Enables the ability to send debug messages to the QMK Toolbox console.
CONSOLE_ENABLE = yes

# Disables the 'Command' feature, which is an older way to check keyboard status.
COMMAND_ENABLE = yes

# Disables N-Key Rollover (the ability to press every key at once). Standard 6-key is default.
NKRO_ENABLE = no

# Disables old-school single-color LED backlighting.
BACKLIGHT_ENABLE = no

# Disables the simpler Underglow-only RGB system in favor of the more advanced Matrix system.
RGBLIGHT_ENABLE = no

# Disables audio/speaker functionality (the Iris Rev 6 does not have a speaker).
AUDIO_ENABLE = no

# Disables support for rotary encoders (knobs). Set to 'yes' if you install them later.
ENCODER_ENABLE = no

# Disables 'Tap Dance', which allows keys to do different things based on how many times they are tapped.
TAP_DANCE_ENABLE = no

# Enables the advanced per-key RGB system used by the Iris Rev 6.
RGB_MATRIX_ENABLE = no

# Enables Link Time Optimization. This significantly reduces the size of your
# compiled firmware so it fits on the chip's limited memory.
LTO_ENABLE = yes

# Enables ability to detect OS on the fly (ie. mac vs windows)
OS_DETECTION_ENABLE = no
