# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I/opt/homebrew/include/
LDFLAGS = -L/opt/homebrew/lib -lcmocka -lpthread

# Automatically detect source files and corresponding object files
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, target/lib/%.o, $(SRC))

# Same for tests
TEST_SRC = $(wildcard tests/*.c)
TEST_OBJ = $(patsubst tests/%.c, target/lib/%.o, $(TEST_SRC))
TEST_SRC_OBJ = $(filter-out target/lib/main.o, $(OBJ))

# Target directories
TARGET_DIR = target/bin
LIB_DIR = target/lib
TARGET = $(TARGET_DIR)/my_project
TEST_TARGET = $(TARGET_DIR)/run_tests

# Default target
all: $(TARGET)
	
compile_commands:
	bear -- make

# Create target from object files
$(TARGET): $(OBJ)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(OBJ) $(LDFLAGS) -o $(TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(TEST_SRC_OBJ)
	@mkdir -p $(TARGET_DIR)

	$(CC) $(TEST_OBJ) $(TEST_SRC_OBJ) $(LDFLAGS) -o $(TEST_TARGET)

# Compile source files into object files
target/lib/%.o: src/%.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/%.o: tests/%.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the lib directory exists
$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

# Clean up build artifacts
clean:
	rm -rf target

# Run the executable
run: $(TARGET)
	@./$(TARGET)

test: $(TEST_TARGET)
	@./$(TEST_TARGET)
