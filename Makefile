# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I/opt/homebrew/include/
LDFLAGS = -L/opt/homebrew/lib -lpthread

# Source files for binaries
BINS = phil
SRC_phil = $(wildcard src/philosophers/*.c)

# Object files for each binary
OBJ_phil = $(patsubst src/philosophers/%.c, target/lib/philosophers/%.o, $(SRC_phil))

# Target directories
TARGET_DIR = target/bin
LIB_DIR = target/lib

# Binaries' targets
TARGETS = $(addprefix $(TARGET_DIR)/, $(BINS))

# Default target
all: $(TARGETS)

compile_commands:
	bear -- make

# Build rules for each binary
$(TARGET_DIR)/phil: $(OBJ_phil)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(OBJ_phil) $(LDFLAGS) -o $@

# Compile source files into object files
target/lib/philosophers/%.o: src/philosophers/%.c | $(LIB_DIR)/philosophers
	$(CC) $(CFLAGS) -c $< -o $@
	
# Ensure the lib subdirectories exist
$(LIB_DIR)/philosophers:
	@mkdir -p $(LIB_DIR)/philosophers
	
# Clean up build artifacts
clean:
	rm -rf target

# Run a specific binary (e.g., make run BIN=phil)
run:
ifeq ($(BIN),)
	@echo "Available binaries:"
	@echo $(BINS) | tr ' ' '\n'
	@echo "Usage: make run BIN=<binary_name> ARGS=\"<arguments>\""
else
	@./$(TARGET_DIR)/$(BIN) $(ARGS)
endif
