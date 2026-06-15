CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
SRC = src/main.c src/MaskGen.c src/Test.c src/Magic.c src/MoveGen.c src/Board.c src/Position.c src/Search.c
OUT = build/main.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm - f $(OUT)