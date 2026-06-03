#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include "Bitboard.h"
#include "Types.h"
#include "MaskGen.h"
#include "Magic.h"

#define ASSERT(cond, msg) if(!(cond)) {printf("Failed Test: %s\n", msg); exit(1);}

void runAllTests();

void KnightTableTest();
void PawnAttackTableTest();
void KingTableTest();
void RookTableTest();
void BishopTableTest();

void RookAttackForBlockersTest();
void BishopAttackForBlockersTest();

#endif