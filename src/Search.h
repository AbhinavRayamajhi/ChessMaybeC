#ifndef SEARCH_H
#define SEARCH_H

#include "Board.h"
#include "MoveGen.h"

int evaluate(const Board* board);
Move findBestMove(Board* board, Depth depth, int* nodes);

#endif