#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include "Board.h"

#define ASSERT(cond, msg) if(!(cond)) {printf("Failed Test: %s\n", msg); exit(1);}

void runAllTableTests();

void KnightTableTest();
void PawnAttackTableTest();
void KingTableTest();
void RookTableTest();
void BishopTableTest();

void RookAttackForBlockersTest();
void BishopAttackForBlockersTest();
void MagicTableTests();

int perft(Board * board, int depth, int rootDepth, int debug, int deeperSearch);
void runAllPerftTests();

#endif