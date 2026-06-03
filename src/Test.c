#include "../include/Test.h"

void runAllTests() {
    printf("%s\n", "Starting Tests...");
    KnightTableTest();
    PawnAttackTableTest();
    KingTableTest();
    RookTableTest();
    BishopTableTest();
    RookAttackForBlockersTest();
    BishopAttackForBlockersTest();
    printf("%s\n", "All tests passed successfully!");
}

void KnightTableTest() {

    ASSERT(popCount(knightTable[A1]) == 2, "Knight on A1 should have 2 attacks.");
    ASSERT(popCount(knightTable[A4]) == 4, "Knight on A4 should have 4 attacks.");
    ASSERT(popCount(knightTable[A8]) == 2, "Knight on A8 should have 2 attacks.");
    ASSERT(popCount(knightTable[E4]) == 8, "Knight on E4 should have 8 attacks.");
    ASSERT(popCount(knightTable[H1]) == 2, "Knight on H1 should have 2 attacks.");
    printf("%s\n", "Knight Table Tests passed...");
}

void PawnAttackTableTest() {

    
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

void KingTableTest() {

    ASSERT(popCount(kingTable[A1]) == 3, "King on A1 should have 3 attacks.");
    ASSERT(popCount(kingTable[A4]) == 5, "King on A4 should have 5 attacks.");
    ASSERT(popCount(kingTable[A8]) == 3, "King on A8 should have 3 attacks.");
    ASSERT(popCount(kingTable[E4]) == 8, "King on E4 should have 8 attacks.");
    ASSERT(popCount(kingTable[H1]) == 3, "King on H1 should have 3 attacks.");
    printf("%s\n", "King Table Tests passed...");
}

void RookTableTest() {

    ASSERT(popCount(rookTable[A1]) == 12, "Rook on A1 should have 12 attacks.");
    ASSERT(popCount(rookTable[A4]) == 11, "Rook on A4 should have 11 attacks.");
    ASSERT(popCount(rookTable[A8]) == 12, "Rook on A8 should have 12 attacks.");
    ASSERT(popCount(rookTable[E4]) == 10, "Rook on E4 should have 10 attacks.");
    ASSERT(popCount(rookTable[H1]) == 12, "Rook on H1 should have 12 attacks.");
    printf("%s\n", "Rook Table Tests passed...");
}

void BishopTableTest() {

    ASSERT(popCount(bishopTable[A1]) == 6, "Bishop on A1 should have 6 attacks.");
    ASSERT(popCount(bishopTable[A4]) == 5, "Bishop on A4 should have 5 attacks.");
    ASSERT(popCount(bishopTable[A8]) == 6, "Bishop on A8 should have 6 attacks.");
    ASSERT(popCount(bishopTable[E4]) == 9, "Bishop on E4 should have 9 attacks.");
    ASSERT(popCount(bishopTable[H1]) == 6, "Bishop on H1 should have 6 attacks.");
    printf("%s\n", "Bishop Table Tests passed...");
}


void RookAttackForBlockersTest() {

    Bitboard bb = 0ULL;
    setSq(bb, A4);
    setSq(bb, E1);

    ASSERT(popCount(rookAttacksForBlockers(bb, A1)) == 7, "With blockers, Rook on A1 should have 7 attacks.");
    ASSERT(popCount(rookAttacksForBlockers(bb, A3)) == 10, "With blockers, Rook on A3 should have 10 attacks.");
    ASSERT(popCount(rookAttacksForBlockers(bb, A8)) == 11, "With blockers, Rook on A8 should have 11 attacks.");
    ASSERT(popCount(rookAttacksForBlockers(bb, H1)) == 10, "With blockers, Rook on H1 should have 10 attacks.");
    ASSERT(popCount(rookAttacksForBlockers(bb, H8)) == 14, "With blockers, Rook on H9 should have 14 attacks.");
    printf("%s\n", "Rook Attacks For Blocker Tests passed...");
}

void BishopAttackForBlockersTest() {
    Bitboard bb = 0ULL;
    setSq(bb, C3);
    setSq(bb, B7);

    ASSERT(popCount(bishopAttacksForBlockers(bb, A1)) == 2, "With blockers, Bishop on A1 should have 2 attacks.");
    ASSERT(popCount(bishopAttacksForBlockers(bb, A3)) == 7, "With blockers, Bishop on A3 should have 7 attacks.");
    ASSERT(popCount(bishopAttacksForBlockers(bb, A8)) == 1, "With blockers, Bishop on A8 should have 1 attacks.");
    ASSERT(popCount(bishopAttacksForBlockers(bb, H1)) == 6, "With blockers, Bishop on H1 should have 6 attacks.");
    ASSERT(popCount(bishopAttacksForBlockers(bb, H8)) == 5, "With blockers, Bishop on H9 should have 5 attacks.");
    printf("%s\n", "Bishop Attacks For Blocker Tests passed...");
}