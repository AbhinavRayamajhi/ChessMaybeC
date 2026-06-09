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

static inline void addHistory(HistoryList* list, History history) {
    list->history[list->end] = history;
    ++list->end;
}

static inline History popHistory(HistoryList* list) {
    --list->end;
    return list->history[list->end];
}

void makeMove(Board* board, History* history, Move move);
void unmakeMove(Board* board, History history);

#endif