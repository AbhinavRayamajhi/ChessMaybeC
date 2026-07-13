#include "TTable.h"

TableEntry TTable[TT_SIZE] = {0};
uint8_t isInTable(Key zobristKey, TableEntry* entry) {

    Key index = zobristKey & (TT_SIZE - 1);

    if (TTable[index].zobristKey == zobristKey) {
        *entry = TTable[index];
        return 1;
    }

    return 0;
}

void writeToTable(Key zobristKey, Move bestMove, Score score, Depth depth,
    BoundType boundType) {

    Key index = zobristKey & (TT_SIZE - 1);

    if (depth < TTable[index].depth) return;

    TTable[index].zobristKey = zobristKey;
    TTable[index].bestMove = bestMove;
    TTable[index].score = score;
    TTable[index].depth = depth;
    TTable[index].boundType = boundType;
}