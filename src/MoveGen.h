#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <stdlib.h>
#include "Bitboard.h"
#include "Board.h"
#include "Magic.h"
#include "MaskGen.h"
#include "Move.h"
#include "Position.h"

typedef enum : uint8_t {
    EVASIONS,
    NON_EVASIONS,
    CAPTURES,
    QUIETS
} GenType;

Bitboard getSquareAttackers(Board* board, Square sq);
void generateLegalMoves(MoveList* moveList, Board* board);

#endif