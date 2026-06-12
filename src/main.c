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
    runAllTableTests();
    
    
    */
    // History h;
    // Board board = getBoardFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1");
    // Move m1 = make(A2, A3, NO_PROMOTION_PIECE, NORMAL);
    // Move m2 = make(B4, B3, NO_PROMOTION_PIECE, NORMAL);
    // Move m3 = make(D2, C1, NO_PROMOTION_PIECE, NORMAL);
    // Move m4 = make(E7, B4, NO_PROMOTION_PIECE, NORMAL);
    // Move m5 = make(B2, B3, NO_PROMOTION_PIECE, NORMAL);
    // makeMove(&board, &h, m1);
    // makeMove(&board, &h, m2);
    // makeMove(&board, &h, m3);
    // makeMove(&board, &h, m4);
    //makeMove(&board, &h, m5);
    // int depth = 5;
    // int debug = 1;
    // int deeperSearch = 0;

    // printf("%d\n", perft(&board, depth, depth, debug, deeperSearch));

    runAllPerftTests();
    return 0;
}