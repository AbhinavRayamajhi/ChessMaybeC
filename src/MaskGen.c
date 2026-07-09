#include "MaskGen.h"

#include "Bitboard.h"
#include "Helpers.h"

Bitboard knightTable[SQ_COUNT];
Bitboard pawnAttackTable[BOTH][SQ_COUNT];
Bitboard kingTable[SQ_COUNT];

Bitboard rookTable[SQ_COUNT];
Bitboard bishopTable[SQ_COUNT];
Bitboard rookAttackTable[SQ_COUNT][4096];
Bitboard bishopAttackTable[SQ_COUNT][512];
Bitboard rays[SQ_COUNT][SQ_COUNT];

void generateKnightMasks() {    
    // Knight attack offsets: +6, +10, +15, +17, -6, -10, -15, -17)
    for (Square sq = A1; sq != NONE; ++sq) {

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

    for (Color c = WHITE; c != BOTH; ++c) {
        for (Square sq = A1; sq != NONE; ++sq) {

            Bitboard mask = 0ULL;
            Bitboard cur = 0ULL;
            setSq(cur, sq);

            if (c == WHITE) {
                mask |= bitboardShift(cur, NORTH_WEST);
                mask |= bitboardShift(cur, NORTH_EAST);
            }
            else {
                mask |= bitboardShift(cur, SOUTH_WEST);
                mask |= bitboardShift(cur, SOUTH_EAST);
            }

            pawnAttackTable[c][sq] = mask;
        }
    }
}

void generateKingMasks() {

    for (Square sq = 0; sq != NONE; ++sq) {

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

    for (Square sq = 0; sq != NONE; ++sq) {

        Bitboard mask = 0ULL;
        
        // Remove edge squares from each direction, not necessary for magic bitboards
        mask |= FILES[fileOf(sq)] & ~RANK_1 & ~RANK_8;
        mask |= RANKS[rankOf(sq)] & ~FILE_A & ~FILE_H;

        // Remove current sq from mask
        popSq(mask, sq);

        rookTable[sq] = mask;
    }
}

void generateBishopMasks() {

    for (Square sq = 0; sq != NONE; ++sq) {

        Bitboard mask = 0ULL;

        int rank = rankOf(sq);
        int file = fileOf(sq);

        // Remove edge squares from each direction, not necessary for magic bitboards
        mask |=  DIAGS[7 + rank - file] & ~RANK_1 & ~RANK_8 & ~FILE_A & ~FILE_H;
        mask |= ANTI_DIAGS[rank + file] & ~RANK_1 & ~RANK_8 & ~FILE_A & ~FILE_H;

        // Remove current sq from mask
        popSq(mask, sq);

        bishopTable[sq] = mask;
    }
}

Bitboard computeRay(Square sq, Direction dir) {

	Bitboard res = 0ULL;
	Bitboard cur = boardFromSq(sq);

	while ((cur = bitboardShift(cur, dir))) {
		res |= cur;
	}

	return res;
}

Bitboard getRays(Square sq1, Square sq2) {

	int rankDiff = rankOf(sq2) - rankOf(sq1);
	int fileDiff = fileOf(sq2) - fileOf(sq1);

	if (fileDiff == 0) {
		if (rankDiff > 0) return computeRay(sq1, NORTH) & computeRay(sq2, SOUTH);
		if (rankDiff < 0) return computeRay(sq1, SOUTH) & computeRay(sq2, NORTH);
	}

	if (rankDiff == 0) {
		if (fileDiff > 0) return computeRay(sq1, EAST) & computeRay(sq2, WEST);
		if (fileDiff < 0) return computeRay(sq1, WEST) &  computeRay(sq2, EAST);
	}

	if (rankDiff == fileDiff) {
		if (rankDiff > 0) return computeRay(sq1, NORTH_EAST) & computeRay(sq2, SOUTH_WEST);
		if (rankDiff < 0) return computeRay(sq1, SOUTH_WEST) & computeRay(sq2, NORTH_EAST);
	}
	
	if (rankDiff == -fileDiff) {
		if (rankDiff > 0) return computeRay(sq1, NORTH_WEST) & computeRay(sq2, SOUTH_EAST);
		if (rankDiff < 0) return  computeRay(sq1, SOUTH_EAST) & computeRay(sq2, NORTH_WEST);
    }
    
    return 0ULL;
}

void generateRays() {

    for (Square sq1 = A1; sq1 != NONE; ++sq1) {

        for (Square sq2 = A1; sq2 != NONE; ++sq2) {

            rays[sq1][sq2] = getRays(sq1, sq2);
        }
    }
}

void initMasks() {
    
    generateKnightMasks();
    generatePawnAttackMasks();
    generateKingMasks();
    generateRookMasks();
    generateBishopMasks();
    generateRays();
}

Bitboard singlePawnPush(Bitboard pawns, Color sideToMove, Bitboard occ) {

    if (sideToMove == WHITE) {
        return (pawns << 8) & ~occ;
    }

    return (pawns >> 8) & ~occ;
}

Bitboard doublePawnPush(Bitboard pawns, Color sideToMove, Bitboard occ) {

    if (sideToMove == WHITE) {
        return singlePawnPush(singlePawnPush(pawns & RANK_2, WHITE, occ), WHITE, occ);
    }

    return singlePawnPush(singlePawnPush(pawns & RANK_7, BLACK, occ), BLACK, occ);
}

Bitboard pawnLeftAttack(Bitboard pawns, Color sideToMove) {

    if (sideToMove == WHITE) {
        return (pawns & ~FILE_A) << 7;
    }

    return (pawns & ~FILE_A) >> 9;
}

Bitboard pawnRightAttack(Bitboard pawns, Color sideToMove) {

    if (sideToMove == WHITE) {
        return (pawns & ~FILE_H) << 9;
    }

    return (pawns & ~FILE_H) >> 7;
}