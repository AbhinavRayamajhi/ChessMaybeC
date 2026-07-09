#include "MaskGen.h"
#include "Test.h"
#include "Magic.h"
#include "Board.h"
#include "Search.h"
#include "Move.h"
#include "Position.h"
#include "Zobrist.h"
#include "TTable.h"

Zobrist ZOBRIST;

int main() {

    initMasks();
    initMagicTables();
    initZobrist(&ZOBRIST);

    runAllPerftTests();

    // Board board = getBoardFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    // History h;
    // makeMove(&board, &h, create(B4, B1, NO_PROMOTION_PIECE, NORMAL));
    // makeMove(&board, &h, create(D6, D5, NO_PROMOTION_PIECE, NORMAL));
    // makeMove(&board, &h, create(B1, G1, NO_PROMOTION_PIECE, NORMAL));
    // makeMove(&board, &h, create(H4, G5, NO_PROMOTION_PIECE, NORMAL));
    // makeMove(&board, &h, create(G2, G4, NO_PROMOTION_PIECE, NORMAL));

    // printBoard(&board);

    // int depth = 1;
    // int nodes = perft(&board, depth, depth, 1, 0);
    // printf("\n%d\n", nodes);

    // return 0;
}
