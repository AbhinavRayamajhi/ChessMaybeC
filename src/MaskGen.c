#include "../include/MaskGen.h"

Bitboard knightTable[64];
Bitboard pawnAttackTable[128];
Bitboard kingTable[64];
Bitboard rookTable[64];
Bitboard bishopTable[64];

void generateKnightMasks() {    
    // Knight attack offsets: +6, +10, +15, +17, -6, -10, -15, -17)
    for (int sq = 0; sq < 64; ++sq) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        mask |= (cur & ~FILE_H )<< 17; // NORTH + NORTH + EAST
        mask |= (cur & ~FILE_A) << 15; // NORTH + NORTH + WEST
        mask |= (cur & ~FILE_G & ~FILE_H) << 10; // NORTH + EAST + EAST 
        mask |= (cur & ~FILE_A & ~FILE_B) <<  6; // NORTH + WEST + WEST
        mask |= (cur & ~FILE_A) >> 17; // SOUTH + SOUTH + WEST
        mask |= (cur & ~FILE_H) >> 15; // SOUTH + SOUTH + EAST
        mask |= (cur & ~FILE_A & ~FILE_B) >> 10; // SOUTH + WEST + WEST
        mask |= (cur & ~FILE_G & ~FILE_H) >>  6; // SOUTH + EAST + EAST

        knightTable[sq] = mask;
    }
}

void generatePawnAttackMasks() {
    // For black pawns index sq + 64, gives us the correct masks
    for (int sq = 0; sq < 128; ++sq) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        if(sq < 64) {
            mask |= (cur & ~FILE_A) << 7; // NORTH_WEST 
            mask |= (cur & ~FILE_H) << 9; // NORTH_EAST
        }
        else {
            mask |= (cur & ~FILE_A) >> 9; // SOUTH_WEST
            mask |= (cur & ~FILE_H) >> 7; // SOUTH_EAST
        }

        pawnAttackTable[sq] = mask;
    }
}

void generateKingMasks() {

    for (int sq = 0; sq < 64; ++sq) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        mask |= (cur & ~FILE_A) << 7; // NORTH_WEST
        mask |= cur << 8; // NORTH
        mask |= (cur & ~FILE_H) << 9; // NORTH_EAST
        mask |= (cur & ~FILE_A) >> 1; // EAST
        mask |= (cur & ~FILE_H) << 1; // WEST
        mask |= (cur & ~FILE_A) >> 9; // SOUTH_WEST
        mask |= cur >> 8; // SOUTH
        mask |= (cur & ~FILE_H) >> 7; // SOUTH_EAST

        kingTable[sq] = mask;
    }
}

void generateRookMasks() {

    for (int sq = 0; sq < 64; ++sq) {

        Bitboard mask = 0ULL;
        
        // Remove edge squares from each direction, not necessary for magic bitboards
        mask |= FILES[sq % 8] & ~RANK_1 & ~RANK_8;
        mask |= RANKS[sq / 8] & ~FILE_A & ~FILE_H;

        // Remove current sq from mask
        popSq(mask, sq);

        rookTable[sq] = mask;
    }
}

void generateBishopMasks() {

    for (int sq = 0; sq < 64; ++sq) {

        Bitboard mask = 0ULL;

        int rank = sq / 8;
        int file = sq % 8;

        // Remove edge squares from each direction, not necessary for magic bitboards
        mask |= DIAGS[7 + rank - file] & ~RANK_1 & ~RANK_8 & ~FILE_A & ~FILE_H;
        mask |= ANTI_DIAGS[rank + file] & ~RANK_1 & ~RANK_8 & ~FILE_A & ~FILE_H;

        // Remove current sq from mask
        popSq(mask, sq);

        bishopTable[sq] = mask;
    }
}