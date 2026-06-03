#include "../include/MaskGen.h"
#include "../include/Test.h"

int main() {

    generateKnightMasks();
    generatePawnAttackMasks();
    generateKingMasks();
    generateRookMasks();
    generateBishopMasks();

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
    */

    runAllTests();
    return 0;
}