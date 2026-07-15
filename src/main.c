#include "MaskGen.h"
#include "Test.h"
#include "Magic.h"
#include "Board.h"
#include "Search.h"
#include "Move.h"
#include "Position.h"
#include "Zobrist.h"
#include "TTable.h"
#include "UCI.h"

Zobrist ZOBRIST;

int main() {

    initMasks();
    initMagicTables();
    initZobrist(&ZOBRIST);

    uciLoop();
    // runAllPerftTests();

    // Board board = getInitialBoard(); // getBoardFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // History h;
    // makeMove(&board, &h, create(E2, E4, NO_PROMOTION_PIECE, NORMAL));
    // printf("%d\n", board.enPassantSq);
    // perft(&board, 1, 1, 1, 0);
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
