# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I/opt/homebrew/include/
LDFLAGS = -L/opt/homebrew/lib -lpthread

# Source files for binaries
BINS = phil rw pro
SRC_phil = $(wildcard src/philosophers/*.c)
SRC_rw = $(wildcard src/reads-writes/*.c)
SRC_pro = $(wildcard src/producer/*.c)

# Object files for each binary
OBJ_phil = $(patsubst src/philosophers/%.c, target/lib/philosophers/%.o, $(SRC_phil))
OBJ_rw = $(patsubst src/reads-writes/%.c, target/lib/reads-writes/%.o, $(SRC_rw))
OBJ_pro = $(patsubst src/producer/%.c, target/lib/producer/%.o, $(SRC_pro))

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

$(TARGET_DIR)/rw: $(OBJ_rw)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(OBJ_rw) $(LDFLAGS) -o $@

$(TARGET_DIR)/pro: $(OBJ_pro)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(OBJ_pro) $(LDFLAGS) -o $@

# Compile source files into object files
target/lib/philosophers/%.o: src/philosophers/%.c | $(LIB_DIR)/philosophers
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/reads-writes/%.o: src/reads-writes/%.c | $(LIB_DIR)/reads-writes
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/producer/%.o: src/producer/%.c | $(LIB_DIR)/producer
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the lib subdirectories exist
$(LIB_DIR)/philosophers:
	@mkdir -p $(LIB_DIR)/philosophers

$(LIB_DIR)/reads-writes:
	@mkdir -p $(LIB_DIR)/reads-writes

$(LIB_DIR)/producer:
	@mkdir -p $(LIB_DIR)/producer

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
