# Template Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g

# Source and target
SRC = main.c
TARGET = out
PROGRAM = main.elf

# Default target to build the program
all: $(TARGET)/$(PROGRAM)

# Rule to build the executable
$(TARGET)/$(PROGRAM): $(SRC)
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)/$(PROGRAM)

# Run the program after building
run: $(TARGET)/$(PROGRAM)
	./$(TARGET)/$(PROGRAM)

# Clean up
clean:
	rm -rf $(TARGET)

