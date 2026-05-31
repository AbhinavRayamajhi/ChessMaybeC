#include "../include/MaskGen.h"

void generateKnightMasks(Bitboard* knightTable) {    
    // Knight attack offsets: +6, +10, +15, +17, -6, -10, -15, -17)
    for (int sq = 0; sq < 64; sq++) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        mask |= (cur & ~FILE_H )<< 17;
        mask |= (cur & ~FILE_A) << 15;
        mask |= (cur & ~FILE_G & ~FILE_H) << 10;
        mask |= (cur & ~FILE_A & ~FILE_B) <<  6;
        mask |= (cur & ~FILE_A) >> 17;
        mask |= (cur & ~FILE_H) >> 15;
        mask |= (cur & ~FILE_A & ~FILE_B) >> 10;
        mask |= (cur & ~FILE_G & ~FILE_H) >>  6;

        *knightTable++ = mask;
    }
}

void generatePawnAttackMasks(Bitboard* pawnAttackTable){
    // For black squares index sq + 64, gives us the correct masks
    for (int sq = 0; sq < 128; sq++) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        if(sq < 64) {
            mask |= (cur & ~FILE_A) << 7;
            mask |= (cur & ~FILE_H) << 9;
        }
        else {
            mask |= (cur & ~FILE_A) >> 9;
            mask |= (cur & ~FILE_H) >> 7;
        }

        *pawnAttackTable++ = mask;
    }
}

void generatePawnPushMasks(Bitboard* pawnTable) {
    // For black squares index sq + 64, gives us the correct masks
    for(int sq = 0; sq < 128; sq++) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);
    }
}

void generateKingMasks(Bitboard* kingTable) {

    for (int sq = 0; sq < 64; sq++) {

        Bitboard mask = 0ULL;
        Bitboard cur = 0ULL;
        setSq(cur, sq);

        mask |= (cur & ~FILE_A) << 7;
        mask |= cur << 8;
        mask |= (cur & ~FILE_H) << 9;
        mask |= (cur & ~FILE_A) >> 1;
        mask |= (cur & ~FILE_H) << 1;
        mask |= (cur & ~FILE_A) >> 9;
        mask |= cur >> 8;
        mask |= (cur & ~FILE_H) >> 7;

        *kingTable++ = mask;
    }
}