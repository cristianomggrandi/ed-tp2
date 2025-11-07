# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lm
TARGET = tp2.out
TARGET_WIN = tp2.exe

# Directories
SRC_DIR = src
INC_DIR = include

# Source files
SOURCES = $(SRC_DIR)/main.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Windows executable target
windows: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET_WIN) $(LDFLAGS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET_WIN)

# Phony targets
.PHONY: all clean windows
