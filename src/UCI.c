#include "UCI.h"

#include <stdio.h>
#include <string.h>

#include "Board.h"
#include "Move.h"
#include "Position.h"
#include "Search.h"
#include "Timing.h"

// static buffer size for char* that can hold around 1400 - 1600 moves
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
        else if (!strcmp(buffer, "stop") || !strcmp(buffer, "ponderhit")) {

            KEEP_SEARCHING = 0;
        }
        // Not part of UCI commands, prints ASCII board
        else if (!strcmp(buffer, "print")) {

            printBoard(&board);
            fflush(stdout);
        }
    }
}

static inline Move stringToMove(Board* board, const char* move) {

    Square start = sqFromRF(move[0], move[1]);
    Square target = sqFromRF(move[2], move[3]);

    Piece promPiece = NO_PROMOTION_PIECE;
    MoveType moveType = NORMAL;
    
    if (move[4] != '\0') {
        moveType = PROMOTION;
        promPiece = findPromotionPiece(move[4]) - 1; // Subtract one here to convert piece into promotion piece 0-3 for move packing
    }
    else {
        Piece piece = pieceOf(board->pieceSqs[start]);

        if (piece == KING && abs((int)start - (int)target) == 2) {
            moveType = CASTLING;
        }
        else if (target == board->enPassantSq && piece == PAWN) {
            moveType = EN_PASSANT;
        }
    }

    return create(start, target, promPiece, moveType);
}

static void parsePosition(char* buffer, Board* board) {

    if (strstr(buffer, "fen") != NULL) {
        *board = getBoardFromFen(strstr(buffer, "fen") + 4); // +4 to skip "fen "
    }

    if (strstr(buffer, "startpos") != NULL) {
        *board = getInitialBoard();
    }
    // Currently there is no move verification we are trusting that the other party is sending correct moves
    // Our game manager, when we make it, maybe can verify move before sending it to engine
    if (strstr(buffer, "moves") != NULL) {

        char* temp = strstr(buffer, "moves") + 6;
        char move[6];

        History h;
        while (temp != NULL && *temp != '\0') {

            for (int i = 0; i < 4; ++i) {

                move[i] = *temp++;
            }
            move[4] = *temp == '\0' || *temp == ' ' ? '\0' : *temp++;
            move[5] = '\0';

            makeMove(board, &h, stringToMove(board, move));

            while (*temp == ' ') ++temp;
        }
    }
}

static void parseGo(char* buffer, Board* board) {

    // remove "go " from buffer
    strtok(buffer, " ");

    uint64_t start = getCurrentTime();

    uint64_t wtime = 0;
    uint64_t btime = 0;
    uint64_t winc = 0;
    uint64_t binc = 0;
    uint64_t movestogo = 0;

    Depth depth = 6; // fixed depth for now
    uint64_t nodes = 0;
    uint64_t movetime = 0;

    // Global variable in search that will constantly be checked by search to stop if signal is sent
    KEEP_SEARCHING = 0;

    char* token;
    while ((token = strtok(NULL, " ")) != NULL) {

        if (!strcmp(token, "wtime")) {
            wtime = atoi(strtok(NULL, " "));
        }
        else if (!strcmp(token, "btime")) {
            btime = atoi(strtok(NULL, " "));
        }
        else if (!strcmp(token, "winc")) {
            winc = atoi(strtok(NULL, " "));
        }
        else if (!strcmp(token, "binc")) {
            binc = atoi(strtok(NULL, " "));
        }
        else if (!strcmp(token, "movestogo")) {
            movestogo= atoi(strtok(NULL, " "));
        }

        else if (!strcmp(token, "depth")) {
            depth = atoi(strtok(NULL, " "));
        }
        else if (!strcmp(token, "nodes")) {
            nodes = atoi(strtok(NULL, " "));
        }
        else if (!strcmp(token, "movetime")) {
            movetime= atoi(strtok(NULL, " "));
        }

        else if (!strcmp(token, "infinite") || !strcmp(token, "ponder")) {
            KEEP_SEARCHING = 1;
        }
    }

    uint64_t tempNodeCount = 0;
    Move bestMove = findBestMove(board, depth, &tempNodeCount);

    printf("%s", "bestmove ");
    printMove(bestMove, board->sideToMove);
    
    printf("%c", '\n');
    fflush(stdout);
}