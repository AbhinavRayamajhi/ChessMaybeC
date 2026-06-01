/*
* Definitions for bitboard, squares, specific ranks and files, colors
*/
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define Bitboard uint64_t

#define FILE_A 0x0101010101010101
#define FILE_B 0x0202020202020202
#define FILE_C 0x0404040404040404
#define FILE_D 0x0808080808080808
#define FILE_E 0x1010101010101010
#define FILE_F 0x2020202020202020
#define FILE_G 0x4040404040404040
#define FILE_H 0x8080808080808080

#define RANK_1 0x00000000000000FF
#define RANK_2 0x000000000000FF00
#define RANK_3 0x0000000000FF0000
#define RANK_4 0x00000000FF000000
#define RANK_5 0x000000FF00000000
#define RANK_6 0x0000FF0000000000
#define RANK_7 0x00FF000000000000
#define RANK_8 0xFF00000000000000

typedef enum {
	A1 = 0, B1, C1, D1, E1, F1, G1, H1,
	    A2, B2, C2, D2, E2, F2, G2, H2,
	    A3, B3, C3, D3, E3, F3, G3, H3,
	    A4, B4, C4, D4, E4, F4, G4, H4,
	    A5, B5, C5, D5, E5, F5, G5, H5,
	    A6, B6, C6, D6, E6, F6, G6, H6,
	    A7, B7, C7, D7, E7, F7, G7, H7,
	    A8, B8, C8, D8, E8, F8, G8, H8
} Square;

static const Bitboard FILES[] = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
static const Bitboard RANKS[] = {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};

typedef enum {
	NORTH = 8,
	SOUTH = -8,
	EAST = 1,
	WEST = -1,
	NORTH_EAST = 9,
	NORTH_WEST = 7,
	SOUTH_EAST = -7,
	SOUTH_WEST = -9
} Direction;

typedef enum {
    WHITE = 0,
    BLACK
} Color;

#endif