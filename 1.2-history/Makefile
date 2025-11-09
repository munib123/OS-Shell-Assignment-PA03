CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC = src/main.c src/shell.c src/execute.c
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

TARGET = bin/myshell

all: dirs $(TARGET)

dirs:
	mkdir -p obj bin

obj/%.o: src/%.c include/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

clean:
	rm -rf obj/*.o $(TARGET)

run: all
	./$(TARGET)