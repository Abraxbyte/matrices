# Project
APP_NAME    := matrices
LIB_NAME    := matrix

# Layout
SRC_DIR     := src
INC_DIR     := include
BUILD_DIR   := build
BIN_DIR     := bin
LIB_DIR     := lib
TEST_DIR    := tests

# Tools
CC          := gcc

# Flags
CSTD        := c17
CFLAGS      := -std=$(CSTD) -Wall -Wextra -Wpedantic -O2 -I$(INC_DIR) -MMD -MP

# Sources
LIB_SRC     := $(wildcard $(SRC_DIR)/matrix_*.c)
LIB_OBJ     := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))
APP_SRC     := $(SRC_DIR)/main.c
APP_OBJ     := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(APP_SRC))
DEP         := $(LIB_OBJ:.o=.d) $(APP_OBJ:.o=.d)
LIB_STATIC  := $(LIB_DIR)/lib$(LIB_NAME).a
APP_BIN     := $(BIN_DIR)/$(APP_NAME)
TEST_BIN    := $(BIN_DIR)/tests_matrix

.PHONY: all clean test

all: $(APP_BIN)

$(APP_BIN): $(LIB_STATIC) $(APP_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(APP_OBJ) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -o $@

$(LIB_STATIC): $(LIB_OBJ) | $(LIB_DIR)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB_STATIC)
	$(CC) $(filter-out -MMD -MP,$(CFLAGS)) -o $(TEST_BIN) $(TEST_DIR)/*.c -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)
	$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)

$(BUILD_DIR) $(BIN_DIR) $(LIB_DIR):
	mkdir -p $@

-include $(DEP)
