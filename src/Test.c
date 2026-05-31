#include "../include/Test.h"

void runAllTests(Bitboard* knightTable, Bitboard* pawnAttackTable, Bitboard* kingTable) {
    printf("%s\n", "Starting Tests...");
    KnightTableTest(knightTable);
    PawnAttackTableTest(pawnAttackTable);
    KingTableTest(kingTable);
    printf("%s\n", "All tests passed successfully!");
}

void KnightTableTest(Bitboard* knightTable) {

    ASSERT(popCount(knightTable[A1]) == 2, "Knight on A1 should have 2 attacks.");
    ASSERT(popCount(knightTable[A4]) == 4, "Knight on A4 should have 4 attacks.");
    ASSERT(popCount(knightTable[A8]) == 2, "Knight on A8 should have 2 attacks.");
    ASSERT(popCount(knightTable[E4]) == 8, "Knight on E4 should have 8 attacks.");
    ASSERT(popCount(knightTable[H1]) == 2, "Knight on H1 should have 2 attacks.");
    printf("%s\n", "Knight Table Tests passed...");
}

void PawnAttackTableTest(Bitboard* pawnAttackTable) {

    
    ASSERT(popCount(pawnAttackTable[A2]) == 1, "White Pawn on A2 should have 1 attacks.");
    ASSERT(popCount(pawnAttackTable[E2]) == 2, "White Pawn on E2 should have 2 attacks.");
    ASSERT(popCount(pawnAttackTable[E7]) == 2, "White Pawn on E7 should have 2 attacks.");
    ASSERT(popCount(pawnAttackTable[H7]) == 1, "White Pawn on H7 should have 1 attacks.");
    ASSERT(popCount(pawnAttackTable[A2 + 64]) == 1, "Black Pawn on A2 should have 1 attacks.");
    ASSERT(popCount(pawnAttackTable[E2 + 64]) == 2, "Black Pawn on E2 should have 2 attacks.");
    ASSERT(popCount(pawnAttackTable[E7 + 64]) == 2, "Black Pawn on E7 should have 2 attacks.");
    ASSERT(popCount(pawnAttackTable[H7 + 64]) == 1, "Black Pawn on H7 should have 1 attacks.");
    printf("%s\n", "Pawn Attack Table Tests passed...");
}

void KingTableTest(Bitboard* kingTable) {

    ASSERT(popCount(kingTable[A1]) == 3, "King on A1 should have 3 attacks.");
    ASSERT(popCount(kingTable[A4]) == 5, "King on A4 should have 5 attacks.");
    ASSERT(popCount(kingTable[A8]) == 3, "King on A8 should have 3 attacks.");
    ASSERT(popCount(kingTable[E4]) == 8, "King on E4 should have 8 attacks.");
    ASSERT(popCount(kingTable[H1]) == 3, "King on H1 should have 3 attacks.");
    printf("%s\n", "King Table Tests passed...");
}
