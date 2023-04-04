CC = gcc
CFLAGS = -Wall -Wextra -pedantic# -std=c99
LDFLAGS = -ldl

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# List of objects files
OBJS = $(wildcard $(*.o))

# List of shared libraries to build
LIBS = $(LIB_DIR)/libcodecA.so $(LIB_DIR)/libcodecB.so

# Default target: build all libraries and the main program
all: $(LIBS) encode decode cmp copy stshell

cmp: cmp.c
	$(CC) $(CFLAGS) $< -o $@

copy: copy.c
	$(CC) $(CFLAGS) $< -o $@

# The shell like program
stshell: shell.c
	$(CC) $(CFLAGS) $< -o $@

# Build shared library for codec A
$(LIB_DIR)/libcodecA.so: $(BUILD_DIR)/codecA.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

# Build shared library for codec B
$(LIB_DIR)/libcodecB.so: $(BUILD_DIR)/codecB.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

# Build the main programs
encode: $(BUILD_DIR)/encode.o $(BUILD_DIR)/encode.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

decode: $(BUILD_DIR)/decode.o $(BUILD_DIR)/decode.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Build object files from source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build directory and all generated files
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) codec cmp copy stshell encode decode comp

# Phony targets
.PHONY: all clean
