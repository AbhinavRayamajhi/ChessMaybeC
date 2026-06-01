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

void generateKnightMasks(Bitboard* knightTable);
void generatePawnAttackMasks(Bitboard* pawnAttackTable);
void generatePawnPushMasks(Bitboard* pawnTable);
void generateKingMasks(Bitboard *kingTable);
void generateRookMasks(Bitboard *rookTable);

#endif