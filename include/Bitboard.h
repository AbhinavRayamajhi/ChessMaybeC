/*
* Macros and helper functions for bitboard manipulation
*/

#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdio.h>
#include "Types.h"

#define popSq(bb, sq) bb &= ~(1ULL << (sq))
#define setSq(bb, sq) bb |= (1ULL << (sq))
#define getSq(bb, sq) ((bb >> (sq)) & 1ULL)
#define clearLSB(bb) bb &= bb -  1
#define getLSB(bb) __builtin_ctzll(bb)
#define popCount(bb) __builtin_popcountll(bb)

static void printBitboard(Bitboard bb) {

    for (int rank = 7; rank >= 0; --rank){

        printf("%d   ", rank + 1);
        for(int file = 0; file < 8; ++file) {

            int sq = rank * 8 + file;
            printf("%c ", (int)((bb >> sq) & 1) + '0');
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
}

#endif