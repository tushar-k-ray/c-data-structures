CC ?= gcc
CFLAGS ?= -Wall -Wextra -Werror -pedantic -std=c11 -Iinclude -g
SANFLAGS ?= -fsanitize=address,undefined

SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
DEMO_DIR = demo
BUILD_DIR = build
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))

DEMO_SRC = $(DEMO_DIR)/main_interactive.c
DEMO_OBJ = $(BUILD_DIR)/main_interactive.o

DEMO_BIN = $(BIN_DIR)/dsa_demo
TEST_BIN = $(BIN_DIR)/dsa_tests

.PHONY: all test demo clean check dirs

all: dirs $(DEMO_BIN) $(TEST_BIN)

dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Compile library source objects with sanitizer flags for guaranteed safety
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) $(SANFLAGS) -c $< -o $@

# Compile demo object
$(DEMO_OBJ): $(DEMO_SRC) | dirs
	$(CC) $(CFLAGS) $(SANFLAGS) -c $< -o $@

# Compile test objects
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | dirs
	$(CC) $(CFLAGS) $(SANFLAGS) -c $< -o $@

# Link demo binary
$(DEMO_BIN): $(OBJS) $(DEMO_OBJ) | dirs
	$(CC) $(CFLAGS) $(SANFLAGS) $^ -o $@

# Link test binary
$(TEST_BIN): $(OBJS) $(TEST_OBJS) | dirs
	$(CC) $(CFLAGS) $(SANFLAGS) $^ -o $@

# Run automated unit tests under AddressSanitizer
test: $(TEST_BIN)
	@echo "Running comprehensive unit tests with AddressSanitizer..."
	@./$(TEST_BIN)

check: test

# Launch interactive terminal UI
demo: $(DEMO_BIN)
	@./$(DEMO_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
