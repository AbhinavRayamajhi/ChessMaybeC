CC = gcc
CFLAGS = -Wall -Wextra -O3 -DNDEBUG
SRC = src/main.c src/MaskGen.c src/Test.c src/Magic.c src/MoveGen.c src/Board.c src/Position.c src/Search.c src/UCI.c
OUT = build/main.exe

DFLAGS = -g -Wall -Wextra -O0 -fsanitize=address -fsanitize=undefined 
DOUT = build/debug/main.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)
	rm -rf $(DOUT)

debug:
	$(CC) $(DFLAGS) $(SRC) -o $(DOUT)