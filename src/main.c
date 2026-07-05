#include "MaskGen.h"
#include "Test.h"
#include "Magic.h"
#include "Board.h"
#include "Search.h"
#include "Move.h"
#include "Position.h"

int main() {

    initMasks();
    initMagicTables();

    runAllPerftTests();

    // Board board = getBoardFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // History h;
    // makeMove(&board, &h, create(D2, D4, NO_PROMOTION_PIECE, NORMAL));
    // makeMove(&board, &h, create(E7, E5, NO_PROMOTION_PIECE, NORMAL));
    // makeMove(&board, &h, create(C1, E3, NO_PROMOTION_PIECE, NORMAL));

    // int depth = 1;
    // printBoard(&board);
    // int nodes = perft(&board, depth, depth, 1, 1);
    // printf("\n%d\n", nodes);

    return 0;
}