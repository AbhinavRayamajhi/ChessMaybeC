#include "../include/MaskGen.h"
#include "../include/Test.h"

int main() {

    generateKnightMasks(knightTable);
    generatePawnAttackMasks(pawnAttackTable);
    generateKingMasks(kingTable);
    generateRookMasks(rookTable);

    runAllTests();
    return 0;
}