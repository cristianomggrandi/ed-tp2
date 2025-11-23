# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lm
TARGET = bin/tp2.out
TARGET_WIN = tp2.exe

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

# Source files
SOURCES = $(SRC_DIR)/main.c

# Header files
HEADERS = $(INC_DIR)/types.h $(INC_DIR)/util.h $(INC_DIR)/minheap.h

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(BIN_DIR) $(TARGET)

# Cria diretórios se não existirem
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Windows executable target
windows: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET_WIN) $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET_WIN)
	-rmdir $(BIN_DIR) 2>/dev/null || true

# Phony targets
.PHONY: all clean windows
