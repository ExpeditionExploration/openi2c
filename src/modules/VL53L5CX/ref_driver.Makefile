# This Makefile builds the VL53L5CX reference driver as a shared library.

# How to link the C driver and Node-API binds. The other choice is shared.
LINKING=?shared

CC := gcc
AR := ar

BUILD_DIR := .
SRC_FILES := $(shell find ../../vl53l5cx-linux/user/ \
	-path ../../vl53l5cx-linux/user/examples -prune -o \
	-path ../../vl53l5cx-linux/user/test -prune -o \
	-regextype sed -iregex ".*\.c" -print)

OBJ_FILES = $(notdir $(addsuffix .o, $(basename $(SRC_FILES))))
# Set to TRUE to enable logging functions.
LOG_ENABLE = FALSE

CORE_INCLUDE_PATHS = -I../../vl53l5cx-linux/user/uld-driver/inc
PLATFORM_INCLUDE_PATHS = -I../../vl53l5cx-linux/user/platform
INCLUDE_PATH = $(CORE_INCLUDE_PATHS) $(PLATFORM_INCLUDE_PATHS)

BASE_CFLAGS = -Wall -Werror -Wno-missing-braces
CFLAGS_RELEASE = -Os -g0 -fPIC
CFLAGS = $(BASE_CFLAGS) $(CFLAGS_RELEASE) $(INCLUDE_PATH)

vpath %.c $(sort $(dir $(SRC_FILES)))

ifeq ($(LINKING),shared)
all: libvl53l5cx-driver.so
else
all: libvl53l5cx-driver.a
endif

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

$(BUILD_DIR)/libvl53l5cx-driver.so: $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))
	$(CC) -shared -o $@ $^

$(BUILD_DIR)/libvl53l5cx-driver.a: $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))
	$(AR) rcs $@ $^

