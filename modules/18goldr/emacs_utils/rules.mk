MODULE_PATH := $(dir $(lastword $(MAKEFILE_LIST)))
CFLAGS += -I$(MODULE_PATH)include

# Kill Line Feature
ifneq ($(strip $(KILL_LINE_ENABLE)),)
    OPT_DEFS += -DKILL_LINE_ENABLE
    SRC += $(MODULE_PATH)src/kill_line.c
endif

# Select Region Feature
ifneq ($(strip $(SELECT_REGION_ENABLE)),)
    OPT_DEFS += -DSELECT_REGION_ENABLE
    SRC += $(MODULE_PATH)src/select_region.c
endif

# Helpers (Include if EITHER is enabled)
# Logic: We stick the two variables together. If the result is not empty,
# it means at least one of them is set to 'yes'.
ifneq ($(strip $(KILL_LINE_ENABLE)$(SELECT_REGION_ENABLE)),)
    SRC += $(MODULE_PATH)src/helpers.c
endif