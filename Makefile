CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/MaskGen.c src/Test.c
OUT = build/main.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm - f $(OUT)