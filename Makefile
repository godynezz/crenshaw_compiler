CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror

SRC = main.c
DEFS = cradle.h
PROGRAM = main
OUTPUT = out

$(OUTPUT)/$(PROGRAM): $(SRC) $(DEFS)
	mkdir -p $(OUTPUT)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)/$(PROGRAM)

run: ./$(OUTPUT)/$(PROGRAM)
	./$(OUTPUT)/$(PROGRAM)

.PHONY : clean
clean :
	rm -rf $(OUTPUT)