MODULE_PATH := $(dir $(lastword $(MAKEFILE_LIST)))
CFLAGS += -I$(MODULE_PATH)include

# 1. Kill Line Feature
ifneq ($(strip $(ENABLE_KILL_LINE)),)
    OPT_DEFS += -DENABLE_KILL_LINE
    SRC += $(MODULE_PATH)src/kill_line.c
endif

# 2. Select Region Feature
ifneq ($(strip $(ENABLE_SELECT_REGION)),)
    OPT_DEFS += -DENABLE_SELECT_REGION
    SRC += $(MODULE_PATH)src/select_region.c
endif

# 3. Helpers (Include if EITHER is enabled)
# Logic: We stick the two variables together. If the result is not empty, 
# it means at least one of them is set to 'yes'.
ifneq ($(strip $(ENABLE_KILL_LINE)$(ENABLE_SELECT_REGION)),)
    SRC += $(MODULE_PATH)src/helpers.c
endif