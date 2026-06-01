#include "../include/MaskGen.h"
#include "../include/Test.h"

int main() {

    Bitboard knightTable[64];
    generateKnightMasks(knightTable);

    Bitboard pawnAttackTable[128];
    generatePawnAttackMasks(pawnAttackTable);

    Bitboard kingTable[64];
    generateKingMasks(kingTable);

    Bitboard rookTable[64];
    generateRookMasks(rookTable);

    for (int i = 0; i < 8; ++i) {
        printBitboard(RANKS[i]);
    }

    printBitboard(rookTable[A1]);
    printBitboard(rookTable[H8]);

    runAllTests(knightTable, pawnAttackTable, kingTable);
    return 0;
}