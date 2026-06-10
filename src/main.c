#include "../include/MaskGen.h"
#include "../include/Test.h"
#include "../include/Magic.h"
#include "../include/Board.h"

int main() {

    generateKnightMasks();
    generatePawnAttackMasks();
    generateKingMasks();
    generateRookMasks();
    generateBishopMasks();
    initMagicTables();

    /*
    printBitboard(      kingTable[E8]);
    printBitboard(    knightTable[E8]);
    printBitboard(    bishopTable[E8]);
    printBitboard(      rookTable[E8]);
    printBitboard(pawnAttackTable[E8]);
    printBitboard(pawnAttackTable[E8 + 64]);
    
    Bitboard bb = 0ULL;
    setSq(bb, A4);
    setSq(bb, E1);
    printBitboard(rookAttacksForBlockers(bb, A3));

    Bitboard bb = 0ULL;
    setSq(bb, C3);
    setSq(bb, B7);
    printBitboard(bishopAttacksForBlockers(bb, A1));

    Board board = getInitialBoard();

    printBitboard(board.pieces[BLACK][PAWN]  );
    printBitboard(board.pieces[BLACK][KNIGHT]);
    printBitboard(board.pieces[BLACK][BISHOP]);
    printBitboard(board.pieces[BLACK][ROOK]  );
    printBitboard(board.pieces[BLACK][QUEEN] );
    printBitboard(board.pieces[BLACK][KING]  );
    */

    // runAllTests();
    History h;
    Board board = getInitialBoard();
    Move m = make(E2, E3, NO_PROMOTION_PIECE, NORMAL);
    int depth = 5;
    int debug = 1;

    //makeMove(&board, &h, m);
    printf("%d\n", perft(&board, depth, depth, debug));
    return 0;
}