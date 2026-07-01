#include "MaskGen.h"

#include "Bitboard.h"

Bitboard knightTable[64];
Bitboard pawnAttackTable[128];
Bitboard kingTable[64];

Bitboard rookTable[64];
Bitboard bishopTable[64];
Bitboard rookAttackTable[64][4096];
Bitboard bishopAttackTable[64][512];

void generateKnightMasks() {    
    // Knight attack offsets: +6, +10, +15, +17, -6, -10, -15, -17)
    for (int sq = 0; sq < 64; ++sq) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        mask |= bitboardShift(cur, NORTH_NORTH_WEST);
        mask |= bitboardShift(cur, NORTH_NORTH_EAST);

        mask |= bitboardShift(cur, NORTH_WEST_WEST);
        mask |= bitboardShift(cur, NORTH_EAST_EAST);

        mask |= bitboardShift(cur, SOUTH_SOUTH_WEST); 
        mask |= bitboardShift(cur, SOUTH_SOUTH_EAST); 

        mask |= bitboardShift(cur, SOUTH_WEST_WEST); 
        mask |= bitboardShift(cur, SOUTH_EAST_EAST); 

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
            mask |= bitboardShift(cur, NORTH_WEST);
            mask |= bitboardShift(cur, NORTH_EAST); 
        }
        else {
            mask |= bitboardShift(cur, SOUTH_WEST);
            mask |= bitboardShift(cur, SOUTH_EAST); 
        }

        pawnAttackTable[sq] = mask;
    }
}

void generateKingMasks() {

    for (int sq = 0; sq < 64; ++sq) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        mask |= bitboardShift(cur, NORTH_WEST);
        mask |= bitboardShift(cur, NORTH);
        mask |= bitboardShift(cur, NORTH_EAST);

        mask |= bitboardShift(cur, WEST); 
        mask |= bitboardShift(cur, EAST); 

        mask |= bitboardShift(cur, SOUTH_WEST);
        mask |= bitboardShift(cur, SOUTH);
        mask |= bitboardShift(cur, SOUTH_EAST); 

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
        mask |=  DIAGS[7 + rank - file] & ~RANK_1 & ~RANK_8 & ~FILE_A & ~FILE_H;
        mask |= ANTI_DIAGS[rank + file] & ~RANK_1 & ~RANK_8 & ~FILE_A & ~FILE_H;

        // Remove current sq from mask
        popSq(mask, sq);

        bishopTable[sq] = mask;
    }
}

void initMasks() {
    generateKnightMasks();
    generatePawnAttackMasks();
    generateKingMasks();
    generateRookMasks();
    generateBishopMasks();
}

Bitboard singlePawnPush(Bitboard pawns, Color sideToMove, Bitboard occ) {

    if (sideToMove) {
        return (pawns >> 8) & ~occ;
    }

    return (pawns << 8) & ~occ;
}

Bitboard doublePawnPush(Bitboard pawns, Color sideToMove, Bitboard occ) {

    if (sideToMove) {
        return singlePawnPush(singlePawnPush(pawns & RANK_7, BLACK, occ), BLACK, occ);
    }

    return singlePawnPush(singlePawnPush(pawns & RANK_2, WHITE, occ), WHITE, occ);
}

Bitboard pawnLeftAttack(Bitboard pawns, Color sideToMove) {

    if (sideToMove) {
        return (pawns & ~FILE_A) >> 9;
    }

    return (pawns & ~FILE_A) << 7;
}

Bitboard pawnRightAttack(Bitboard pawns, Color sideToMove) {

    if (sideToMove) {
        return (pawns & ~FILE_H) >> 7;
    }

    return (pawns & ~FILE_H) << 9;
}