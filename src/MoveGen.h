#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <stdlib.h>
#include "Types.h"
#include "Board.h"
#include "Move.h"

typedef enum : uint8_t {
    EVASIONS,
    NON_EVASIONS,
    CAPTURES,
    QUIETS,
    ALL
} GenType;

void generateCaptures(MoveList* moveList, Board* board);
void generateLegalMoves(MoveList* moveList, Board* board);

#endif