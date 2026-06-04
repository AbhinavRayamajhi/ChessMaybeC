/*
* Pre computes attack masks for all pieces per sq, runs once during initialization of the engine
*/

#ifndef MASKGEN_H
#define MASKGEN_H

#include "Bitboard.h"
#include "Types.h"

extern Bitboard knightTable[64];
extern Bitboard pawnAttackTable[128];
extern Bitboard kingTable[64];
extern Bitboard rookTable[64];
extern Bitboard bishopTable[64];
extern Bitboard rookAttackTable[64][4096];
extern Bitboard bishopAttackTable[64][512];

void generateKnightMasks();
void generatePawnAttackMasks();
void generatePawnPushMasks();
void generateKingMasks();
void generateRookMasks();
void generateBishopMasks();

#endif