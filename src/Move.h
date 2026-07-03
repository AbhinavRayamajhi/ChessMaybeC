#ifndef MOVE_H
#define MOVE_H

#include <stdio.h>
#include <stdint.h>
#include "Types.h"

//  0 -  5 : start sq
//  6 - 11 : target sq
// 12 - 13 : promotion type: no promotion piece will be passed as Knight
// 14 - 15 : move type(0- normal, 1 - promotion, 2 - en passant, 3 - castling)
typedef uint16_t Move;

static inline Square getStartSq(Move m) { return m & 0x3F; }
static inline Square getTargetSq(Move m) { return (m >> 6) & 0x3F; }
static inline Piece getPromotionPiece(Move m) { return ((m >> 12) & 0b11) + 1; }
static inline MoveType getMoveType(Move m) { return (m >> 14) & 0b11; }

// make sure to pass 0 for prom piece when creating regular moves
static inline Move create(Square start, Square target, Piece promPiece, MoveType moveT) {
    return (Move)(start | (target << 6) | (promPiece << 12) | (moveT << 14));
}

// list of moves definition, theoretical max moves in position is 218 so array size set to 256
typedef struct {    
    Move moveArray[256];
    uint8_t end;
} MoveList;

static inline void addMove(MoveList* moveList, Move m) {

    moveList->moveArray[moveList->end] = m;
    ++moveList->end;
}

static inline void removeMove(MoveList* moveList, int index) {

    moveList->moveArray[index] = moveList->moveArray[--moveList->end];
}

static inline char rankFromSquare(Square sq) {

    return sq / 8 + '1';
}

static inline char fileFromSquare(Square sq) {

    return FILE_MAP[sq % 8];
}

static inline void printMove(Move m) {

    printf("%c%c%c%c", fileFromSquare(getStartSq(m)), rankFromSquare(getStartSq(m)),
        fileFromSquare(getTargetSq(m)), rankFromSquare(getTargetSq(m)));
    if (getMoveType(m) == PROMOTION) {
        printf("%c", PROM_PIECE[getPromotionPiece(m)]);
    }
    printf("\n");
}

#endif