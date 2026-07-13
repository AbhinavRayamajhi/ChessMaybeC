#include "UCI.h"

#include <stdio.h>
#include <string.h>

#include "Board.h"

#define BUFFER_SIZE 8192

static void parsePosition(char* buffer, Board* board);
static void parseGo(char* buffer, Board* board);

void uciLoop() {

    char buffer[BUFFER_SIZE] = { 0 };
    Board board = getInitialBoard();

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {

        // We need to convert the newline character in the buffer to a null character
        char* temp = strchr(buffer, '\n');
        if (temp != NULL) *temp = '\0';

        if (!strcmp(buffer, "uci")) {

            printf("id name ChessMaybeC\n");
            printf("id author Abhinav\n");
            printf("uciok\n");
            fflush(stdout);
        }
        else if (!strcmp(buffer, "isready")) {

            printf("readyok\n");
            fflush(stdout);
        }
        else if (!strcmp(buffer, "ucinewgame")) {

            // need to implement transposition table clearing
        }
        else if (!strncmp(buffer, "setoption", 9)) {

            // setoption is no op because i have not implemented any options
        }
        else if (!strncmp(buffer, "position", 8)) {

            parsePosition(buffer, &board);
        }
        else if (!strncmp(buffer, "go", 2)) {

            parseGo(buffer, &board);
        }
        else if (!strcmp(buffer, "quit")) {

            break;
        }
        // Not part of UCI commands, prints ASCII board
        else if (!strcmp(buffer, "print")) {

            printBoard(&board);
            fflush(stdout);
        }
    }
}

static void parsePosition(char* buffer, Board* board) {


}

static void parseGo(char* buffer, Board* board) {

    
}