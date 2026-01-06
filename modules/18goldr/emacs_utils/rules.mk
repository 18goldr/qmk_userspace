# 1. Get the absolute path of the module directory
#    (dir ...) gives us the path with a trailing slash
MODULE_PATH := $(dir $(lastword $(MAKEFILE_LIST)))

# 2. Add source files using the absolute path
SRC += $(MODULE_PATH)src/kill_line.c
SRC += $(MODULE_PATH)src/helpers.c
SRC += $(MODULE_PATH)src/select_region.c

# 3. FORCE the include path using CFLAGS
#    This bypasses EXTRA_INCDIRS issues by adding the flag manually
CFLAGS += -I$(MODULE_PATH)include