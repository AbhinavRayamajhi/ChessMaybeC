#include "../include/MaskGen.h"
#include "../include/Test.h"

int main() {

    Bitboard knightTable[64];
    generateKnightMasks(knightTable);

    Bitboard pawnAttackTable[128];
    generatePawnAttackMasks(pawnAttackTable);

    Bitboard kingTable[64];
    generateKingMasks(kingTable);

    //printBitboard(kingTable[A1]);
    //printBitboard(kingTable[A4]);
    
    runAllTests(knightTable, pawnAttackTable, kingTable);
    return 0;
}