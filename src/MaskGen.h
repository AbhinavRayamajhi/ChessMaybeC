/*
* Pre computes attack masks for all pieces per sq, runs once during initialization of the engine
*/

#ifndef MASKGEN_H
#define MASKGEN_H

#include "Types.h"

extern Bitboard knightTable[SQ_COUNT];
extern Bitboard pawnAttackTable[SQ_COUNT * 2];
extern Bitboard kingTable[SQ_COUNT];
extern Bitboard rookTable[SQ_COUNT];
extern Bitboard bishopTable[SQ_COUNT];
extern Bitboard rookAttackTable[SQ_COUNT][4096];
extern Bitboard bishopAttackTable[SQ_COUNT][512];
extern Bitboard rays[SQ_COUNT][SQ_COUNT];

void generateKnightMasks();
void generatePawnAttackMasks();
void generatePawnPushMasks();
void generateKingMasks();
void generateRookMasks();
void generateBishopMasks();

void initMasks();

Bitboard singlePawnPush(Bitboard pawns, Color sideToMove, Bitboard occ);
Bitboard doublePawnPush(Bitboard pawns, Color sideToMove, Bitboard occ);
Bitboard pawnLeftAttack(Bitboard pawns, Color sideToMove);
Bitboard pawnRightAttack(Bitboard pawns, Color sideToMove);

#endif