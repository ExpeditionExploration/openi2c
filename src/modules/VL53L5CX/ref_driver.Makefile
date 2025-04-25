# This Makefile builds the VL53L5CX reference driver as a shared library.

CC := gcc
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

all: libvl53l5cx-driver.so

mkdirs:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | mkdirs
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

libvl53l5cx-driver.so: $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))
	$(CC) -shared -o $@ $^

clean_artifacts:
	rm -rf $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) && rm libvl53l5cx-driver.so

