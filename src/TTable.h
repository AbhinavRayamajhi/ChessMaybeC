#ifndef TTABLE_H
#define TTABLE_H

#include "Types.h"
#include "Move.h"

// Total bits : 64 + 16 + 16 + 8 + 8 = 112 bits - 14 bytes + 2 bytes padding = 16 bytes
typedef struct {

    Key zobristKey;
    Move bestMove;
    Score score;
    Depth depth;
    BoundType boundType;
}TableEntry;

#define TT_SIZE (64 * 1024 * 1024 / sizeof(TableEntry))
extern TableEntry TTable[TT_SIZE];

#endif