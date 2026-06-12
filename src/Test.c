#include "../include/Test.h"

void runAllTableTests() {
    printf("%s\n", "Starting Table Tests...");
    KnightTableTest();
    PawnAttackTableTest();
    KingTableTest();
    RookTableTest();
    BishopTableTest();
    RookAttackForBlockersTest();
    BishopAttackForBlockersTest();
    MagicTableTests();

    printf("%s\n", "All table tests passed successfully!");
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

void MagicTableTests() {

    Bitboard bb = 0ULL;
    setSq(bb, A4);
    setSq(bb, E1);

    ASSERT(rookAttacksForBlockers(bb, A1) == getRookAttacks(bb, A1), "Rook attacks not matching");
    ASSERT(rookAttacksForBlockers(bb, A3) == getRookAttacks(bb, A3), "Rook attacks not matching");
    ASSERT(rookAttacksForBlockers(bb, A8) == getRookAttacks(bb, A8), "Rook attacks not matching");
    ASSERT(rookAttacksForBlockers(bb, H1) == getRookAttacks(bb, H1), "Rook attacks not matching");
    ASSERT(rookAttacksForBlockers(bb, H8) == getRookAttacks(bb, H8), "Rook attacks not matching");
    printf("%s\n", "Rook Magic verified..");

    bb = 0ULL;
    setSq(bb, C3);
    setSq(bb, B7);

    ASSERT(bishopAttacksForBlockers(bb, A1) == getBishopAttacks(bb, A1), "Bishop attacks not matching");
    ASSERT(bishopAttacksForBlockers(bb, A3) == getBishopAttacks(bb, A3), "Bishop attacks not matching");
    ASSERT(bishopAttacksForBlockers(bb, A8) == getBishopAttacks(bb, A8), "Bishop attacks not matching");
    ASSERT(bishopAttacksForBlockers(bb, H1) == getBishopAttacks(bb, H1), "Bishop attacks not matching");
    ASSERT(bishopAttacksForBlockers(bb, H8) == getBishopAttacks(bb, H8), "Bishop attacks not matching");
    printf("%s\n", "Bishop Magic verified..");
}

int perft(Board* board, int depth, int rootDepth, int debug, int deeperSearch) {

    if (depth == 0) return 1;

    MoveList moveList;
    moveList.end = 0;

    generateLegalMoves(&moveList, board);
    int numMoves = 0;

    for (int i = 0; i < moveList.end; ++i) {

        History h;
        makeMove(board, &h, moveList.moveArray[i]);
        int node = perft(board, depth - 1, rootDepth, debug, deeperSearch);
        unmakeMove(board, &h);

        if (debug && depth == rootDepth) {
            printMove(moveList.moveArray[i]);
            printf(" : %d\n", node);
        }

        if (deeperSearch && (rootDepth - 1 == depth)) {
            printf("\t");
            printMove(moveList.moveArray[i]);
            printf(" : %d\n", node);
        }

        numMoves += node;
    }

    return numMoves;
}

void runAllPerftTests() {

    struct timeval start, end;
    gettimeofday(&start, NULL);

    Board board = getBoardFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    printf("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n\n");
    printf("Depth 1 %s", ((perft(&board, 1, 1, 0, 0) == 20) ? "Passed\n" : "Failed\n"));
    printf("Depth 2 %s", ((perft(&board, 2, 2, 0, 0) == 400) ? "Passed\n" : "Failed\n"));
    printf("Depth 3 %s", ((perft(&board, 3, 3, 0, 0) == 8902) ? "Passed\n" : "Failed\n"));
    printf("Depth 4 %s", ((perft(&board, 4, 4, 0, 0) == 197281) ? "Passed\n" : "Failed\n"));
    printf("Depth 5 %s", ((perft(&board, 5, 5, 0, 0) == 4865609) ? "Passed\n" : "Failed\n"));
    printf("Depth 6 %s", ((perft(&board, 6, 6, 0, 0) == 119060324) ? "Passed\n\n" : "Failed\n\n"));

    board = getBoardFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    printf("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  0 1\n\n");
    printf("Depth 1 %s", ((perft(&board, 1, 1, 0, 0) == 48) ? "Passed\n" : "Failed\n"));
    printf("Depth 2 %s", ((perft(&board, 2, 2, 0, 0) == 2039) ? "Passed\n" : "Failed\n"));
    printf("Depth 3 %s", ((perft(&board, 3, 3, 0, 0) == 97862) ? "Passed\n" : "Failed\n"));
    printf("Depth 4 %s", ((perft(&board, 4, 4, 0, 0) == 4085603) ? "Passed\n" : "Failed\n"));
    printf("Depth 5 %s", ((perft(&board, 5, 5, 0, 0) == 193690690) ? "Passed\n\n" : "Failed\n\n"));

    board = getBoardFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    printf("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1\n\n");
    printf("Depth 1 %s", ((perft(&board, 1, 1, 0, 0) == 14) ? "Passed\n" : "Failed\n"));
    printf("Depth 2 %s", ((perft(&board, 2, 2, 0, 0) == 191) ? "Passed\n" : "Failed\n"));
    printf("Depth 3 %s", ((perft(&board, 3, 3, 0, 0) == 2812) ? "Passed\n" : "Failed\n"));
    printf("Depth 4 %s", ((perft(&board, 4, 4, 0, 0) == 43238) ? "Passed\n" : "Failed\n"));
    printf("Depth 5 %s", ((perft(&board, 5, 5, 0, 0) == 674624) ? "Passed\n" : "Failed\n"));
    printf("Depth 6 %s", ((perft(&board, 6, 6, 0, 0) == 11030083) ? "Passed\n" : "Failed\n"));
    printf("Depth 7 %s", ((perft(&board, 7, 7, 0, 0) == 178633661) ? "Passed\n\n" : "Failed\n\n"));

    board = getBoardFromFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    printf("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1\n\n");
    printf("Depth 1 %s", ((perft(&board, 1, 1, 0, 0) == 6) ? "Passed\n" : "Failed\n"));
    printf("Depth 2 %s", ((perft(&board, 2, 2, 0, 0) == 264) ? "Passed\n" : "Failed\n"));
    printf("Depth 3 %s", ((perft(&board, 3, 3, 0, 0) == 9467) ? "Passed\n" : "Failed\n"));
    printf("Depth 4 %s", ((perft(&board, 4, 4, 0, 0) == 422333) ? "Passed\n" : "Failed\n"));
    printf("Depth 5 %s", ((perft(&board, 5, 5, 0, 0) == 15833292) ? "Passed\n" : "Failed\n"));
    printf("Depth 6 %s", ((perft(&board, 6, 6, 0, 0) == 706045033) ? "Passed\n\n" : "Failed\n\n"));

    board = getBoardFromFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    printf("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8\n\n");
    printf("Depth 1 %s", ((perft(&board, 1, 1, 0, 0) == 44) ? "Passed\n" : "Failed\n"));
    printf("Depth 2 %s", ((perft(&board, 2, 2, 0, 0) == 1486) ? "Passed\n" : "Failed\n"));
    printf("Depth 3 %s", ((perft(&board, 3, 3, 0, 0) == 62379) ? "Passed\n" : "Failed\n"));
    printf("Depth 4 %s", ((perft(&board, 4, 4, 0, 0) == 2103487) ? "Passed\n" : "Failed\n"));
    printf("Depth 5 %s", ((perft(&board, 5, 5, 0, 0) == 89941194) ? "Passed\n\n" : "Failed\n\n"));
    
    board = getBoardFromFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    printf("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10\n\n");
    printf("Depth 1 %s", ((perft(&board, 1, 1, 0, 0) == 46) ? "Passed\n" : "Failed\n"));
    printf("Depth 2 %s", ((perft(&board, 2, 2, 0, 0) == 2079) ? "Passed\n" : "Failed\n"));
    printf("Depth 3 %s", ((perft(&board, 3, 3, 0, 0) == 89890) ? "Passed\n" : "Failed\n"));
    printf("Depth 4 %s", ((perft(&board, 4, 4, 0, 0) == 3894594) ? "Passed\n" : "Failed\n"));
    printf("Depth 5 %s", ((perft(&board, 5, 5, 0, 0) == 164075551) ? "Passed\n\n" : "Failed\n\n"));

    gettimeofday(&end, NULL);
    int nodes = 1494874546;

    printf("Nodes per second: %ld\n", nodes / (end.tv_sec - start.tv_sec));

}