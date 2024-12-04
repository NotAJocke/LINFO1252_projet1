# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -I/opt/homebrew/include/
LDFLAGS = -L/opt/homebrew/lib -lpthread

BINS = phil rw pro tests

# Source files for binaries
SRC_phil = $(wildcard src/philosophers/*.c)
SRC_rw = $(wildcard src/reads-writes/*.c)
SRC_pro = $(wildcard src/producer/*.c)
SRC_tests = $(wildcard src/tests/*.c)

# Mutex implementation selection
MUTEX_IMPL ?= tts
ifeq ($(MUTEX_IMPL), tts)
	SRC_mutex = src/my_mutex/test_and_test_and_set.c
else
	SRC_mutex = src/my_mutex/test_and_set.c
endif

SRC_mutex += src/my_mutex/semaphore.c
# Object files for each binary
OBJ_phil = $(patsubst src/philosophers/%.c, target/lib/philosophers/%.o, $(SRC_phil))
OBJ_rw = $(patsubst src/reads-writes/%.c, target/lib/reads-writes/%.o, $(SRC_rw))
OBJ_pro = $(patsubst src/producer/%.c, target/lib/producer/%.o, $(SRC_pro))
OBJ_tests = $(patsubst src/tests/%.c, target/lib/tests/%.o, $(SRC_tests))
OBJ_mutex = $(patsubst src/my_mutex/*.c, target/lib/my_mutex/%.o, $(SRC_mutex))

# Target directories
TARGET_DIR = target/bin
LIB_DIR = target/lib

# Binaries' targets
TARGETS = $(addprefix $(TARGET_DIR)/, $(BINS))

# Default target
all: $(TARGETS)
	@echo "Selected mutex source file: $(SRC_mutex)"

compile_commands:
	bear -- make

# Build rules for each binary
$(TARGET_DIR)/phil: $(OBJ_phil) $(OBJ_mutex)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(OBJ_phil) $(OBJ_mutex) $(LDFLAGS) -o $@

$(TARGET_DIR)/rw: $(OBJ_rw) $(OBJ_mutex)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(OBJ_rw) $(OBJ_mutex) $(LDFLAGS) -o $@

$(TARGET_DIR)/tests: $(OBJ_tests) $(OBJ_mutex)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(OBJ_tests) $(OBJ_mutex) $(LDFLAGS) -o $@


$(TARGET_DIR)/pro: $(OBJ_pro) $(OBJ_mutex)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(OBJ_pro) $(OBJ_mutex) $(LDFLAGS) -o $@

# Compile source files into object files
target/lib/philosophers/%.o: src/philosophers/%.c | $(LIB_DIR)/philosophers
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/reads-writes/%.o: src/reads-writes/%.c | $(LIB_DIR)/reads-writes
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/producer/%.o: src/producer/%.c | $(LIB_DIR)/producer
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/tests/%.o: src/tests/%.c | $(LIB_DIR)/tests
	$(CC) $(CFLAGS) -c $< -o $@

target/lib/my_mutex/%.o: src/my_mutex/%.c | $(LIB_DIR)/my_mutex
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the lib subdirectories exist
$(LIB_DIR)/philosophers:
	@mkdir -p $(LIB_DIR)/philosophers

$(LIB_DIR)/reads-writes:
	@mkdir -p $(LIB_DIR)/reads-writes

$(LIB_DIR)/producer:
	@mkdir -p $(LIB_DIR)/producer

$(LIB_DIR)/tests:
	@mkdir -p $(LIB_DIR)/tests


$(LIB_DIR)/my_mutex:
	@mkdir -p (LIB_DIR)/my_mutex

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
