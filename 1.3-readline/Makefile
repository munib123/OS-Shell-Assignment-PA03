CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lreadline          # Add this line for Feature-4

SRC = src/main.c src/shell.c src/execute.c
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

TARGET = bin/myshell

all: dirs $(TARGET)

dirs:
	mkdir -p obj bin

obj/%.o: src/%.c include/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)   # <-- Add $(LDFLAGS) here

clean:
	rm -rf obj/*.o $(TARGET)

run: all
	./$(TARGET)