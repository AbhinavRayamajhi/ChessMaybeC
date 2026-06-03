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

static inline Bitboard bitboardShift(Bitboard bb, Direction dir) {

    switch(dir) {
        case NORTH            : return bb << 8; break;
        case SOUTH            : return bb >> 8; break;
        case EAST             : return (bb & ~FILE_H) << 1; break;
        case WEST             : return (bb & ~FILE_A) >> 1; break;
        case NORTH_EAST       : return (bb & ~FILE_H) << 9; break;
        case NORTH_WEST       : return (bb & ~FILE_A) << 7; break;
        case SOUTH_EAST       : return (bb & ~FILE_H) >> 7; break;
        case SOUTH_WEST       : return (bb & ~FILE_A) >> 9; break;
        case NORTH_NORTH_EAST : return (bb & ~FILE_H) << 17; break;
        case NORTH_NORTH_WEST : return (bb & ~FILE_A) << 15; break;
        case SOUTH_SOUTH_EAST : return (bb & ~FILE_H) >> 15; break;
        case SOUTH_SOUTH_WEST : return (bb & ~FILE_A) >> 17; break;
        case NORTH_EAST_EAST  : return(bb & ~FILE_G & ~FILE_H) << 10; break;
        case NORTH_WEST_WEST  : return(bb & ~FILE_A & ~FILE_B) << 6; break;
        case SOUTH_EAST_EAST  : return(bb & ~FILE_G & ~FILE_H) >> 6; break;
        case SOUTH_WEST_WEST  : return(bb & ~FILE_A & ~FILE_B) >> 10; break;
        default : return 0ULL;
    }
}


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