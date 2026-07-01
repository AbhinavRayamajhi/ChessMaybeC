#ifndef POSITION_H
#define POSITION_H

#include "Bitboard.h"
#include "Board.h"
#include "Types.h"
#include "Move.h"

typedef struct {
    Move prevMove;
    Piece moved;
    Piece captured;
    Square prevEnPassantSq;
    CastlingRights prevCastlingRights;
    int prevHalfMoveClock;
} History;

typedef struct {
    History history[1024];
    uint16_t end;
} HistoryList;

void makeMove(Board* board, History* history, Move move);
void unmakeMove(Board* board, History* history);

#endif