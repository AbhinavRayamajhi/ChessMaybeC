#include "../include/MaskGen.h"
#include "../include/Test.h"

int main() {

    generateKnightMasks();
    generatePawnAttackMasks();
    generateKingMasks();
    generateRookMasks();
    generateBishopMasks();

    runAllTests();
    return 0;
}