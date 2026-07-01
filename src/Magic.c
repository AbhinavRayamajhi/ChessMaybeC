#include "Magic.h"

#include "Bitboard.h"
#include "MaskGen.h"

// Generate 64 bit random num
uint64_t rand64() {
    return ((uint64_t)rand() << 32) | rand();
}

// ANDing 3 random numbers gives us lower active bits for our random num, which is a requirement
// for magic numbers to reduce the size of the lookup 
uint64_t randomMagic() {
    return rand64() & rand64() & rand64();
}

Bitboard rookAttacksForBlockers(Bitboard blockers, int sq) {

    Bitboard attacks = 0ULL;
    Bitboard cur = 0ULL;
    Bitboard ray = 0ULL;
    setSq(cur, sq);

    Direction directions[] = { NORTH, SOUTH, EAST, WEST };

    for (int i = 0; i < 4; ++i) {

        ray = bitboardShift(cur, directions[i]);
        while (ray != 0ULL) {

            attacks |= ray;
            if (ray & blockers) {
                break;
            }

            ray = bitboardShift(ray, directions[i]);
        }
    }

    return attacks;
}

Bitboard bishopAttacksForBlockers(Bitboard blockers, int sq) {

    Bitboard attacks = 0ULL;
    Bitboard cur = 0ULL;
    Bitboard ray = 0ULL;
    setSq(cur, sq);

    Direction directions[] = { NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST };

    for (int i = 0; i < 4; ++i) {

        ray = bitboardShift(cur, directions[i]);
        while (ray != 0ULL) {

            attacks |= ray;
            if (ray & blockers) {
                break;
            }

            ray = bitboardShift(ray, directions[i]);
        }
    }

    return attacks;
}

uint64_t findRookMagicNumber(int sq, int bits) {

    Bitboard mask = rookTable[sq];
    int found = 0;
    uint64_t magic = 0ULL;

    while (!found) {

        // table size hard coded to 4096 because max rook blocker subset is 2^12 = 4096 
        Bitboard hash_checker[4096] = {0ULL};
        magic = randomMagic();
        Bitboard subset = 0ULL;

        do {

            uint64_t magicIndex = (subset * magic) >> (64 - bits);
            Bitboard attack = rookAttacksForBlockers(subset, sq);

            if(hash_checker[magicIndex] && hash_checker[magicIndex] != attack) {
                
                break;
            }

            rookAttackTable[sq][magicIndex] = attack;
            hash_checker[magicIndex] = attack;
            subset = (subset - mask) & mask;

            if(subset == 0ULL) found = 1;
        } while (subset != 0ULL);
    }

    return magic;
}

uint64_t findBishopMagicNumber(int sq, int bits) {

    Bitboard mask = bishopTable[sq];
    int found = 0;
    uint64_t magic = 0ULL;

    while (!found) {

        // table size hard coded to 512 because max bishop blocker subset is 2^9 = 512 
        Bitboard hash_checker[512] = {0ULL};
        magic = randomMagic();
        Bitboard subset = 0ULL;

        do {

            uint64_t magicIndex = (subset * magic) >> (64 - bits);
            Bitboard attack = bishopAttacksForBlockers(subset, sq);

            if(hash_checker[magicIndex] && hash_checker[magicIndex] != attack) {
                
                break;
            }

            bishopAttackTable[sq][magicIndex] = attack;
            hash_checker[magicIndex] = attack;
            subset = (subset - mask) & mask;

            if(subset == 0ULL) found = 1;
        } while (subset != 0ULL);
    }

    return magic;
}

void findAllMagics() {

    printf("Rook Magics:\n");
    for (int sq = 0; sq < 64; ++sq) {

        int bits = popCount(rookTable[sq]);
        uint64_t magic = findRookMagicNumber(sq, bits);
        printf("0x%lxULL, \n", magic);
    }
    
    printf("Bishop Magics:\n");
    for (int sq = 0; sq < 64; ++sq) {
        int bits = popCount(bishopTable[sq]);
        uint64_t magic = findBishopMagicNumber(sq, bits);
        printf("0x%lxULL, \n", magic);
    }
}

void initMagicTables() {

    for (int sq = 0; sq < 64; ++sq) {

        int rookBits = popCount(rookTable[sq]);
        int bishopBits = popCount(bishopTable[sq]);

        Bitboard subset = 0ULL;
        do {
            uint64_t rookIndex = (subset * ROOK_MAGICS[sq]) >> (64 - rookBits);
            rookAttackTable[sq][rookIndex] = rookAttacksForBlockers(subset, sq);
            subset = (subset - rookTable[sq]) & rookTable[sq];
        } while (subset != 0ULL);

        subset = 0ULL;
        do {
            uint64_t bishopIndex = (subset * BISHOP_MAGICS[sq]) >> (64 - bishopBits);
            bishopAttackTable[sq][bishopIndex] = bishopAttacksForBlockers(subset, sq);
            subset = (subset - bishopTable[sq]) & bishopTable[sq];
        } while (subset != 0ULL);
    }
}