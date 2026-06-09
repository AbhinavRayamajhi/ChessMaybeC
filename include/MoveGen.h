#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <stdlib.h>
#include "Board.h"
#include "Magic.h"
#include "MaskGen.h"
#include "Move.h"
#include "Position.h"

Bitboard getRookAttacks(Bitboard occ, int sq);
Bitboard getBishopAttacks(Bitboard occ, int sq);

void enumerateKnightMoves(MoveList* moveList, Board* board);
void enumerateKingMoves(MoveList* moveList, Board* board);
void enumerateRookMoves(MoveList* moveList, Board *board);
void enumerateBishopMoves(MoveList* moveList, Board *board);
void enumerateQueenMoves(MoveList* moveList, Board* board);
void enumeratePawnMoves(MoveList* moveList, Board* board);

void generateLegalMoves(MoveList* moveList, HistoryList* historyList, Board* board);

#endif