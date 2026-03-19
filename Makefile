CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lm

SRC_DIR := src
BUILD_DIR := build
BIN := bin

SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')

OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

all: $(BIN)

$(BIN): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)

include $(OBJ:.o=.d)
