#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include "Bitboard.h"
#include "Types.h"
#define ASSERT(cond, msg) if(!(cond)) {printf("Failed Test: %s\n", msg); exit(1);}

void runAllTests(Bitboard* knightTable, Bitboard* pawnAttackTable, Bitboard* kingTable);
void KnightTableTest(Bitboard* knightTable);
void PawnAttackTableTest(Bitboard* pawnAttackTable);
void KingTableTest(Bitboard* kingTable);

#endif