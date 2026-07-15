#ifndef SEARCH_H
#define SEARCH_H

#include "Board.h"
#include "MoveGen.h"

// global to control search from outside
static uint8_t KEEP_SEARCHING = 1;

int evaluate(const Board* board);
Move findBestMove(Board* board, Depth depth, uint64_t* nodes);

#endif