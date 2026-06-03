#include "../include/Magic.h"

Bitboard rookAttacksForBlockers(Bitboard blockers, int sq) {

    Bitboard attacks = 0ULL;
    Bitboard cur = 0ULL;
    Bitboard ray = 0ULL;
    setSq(cur, sq);

    ray = bitboardShift(cur, NORTH);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, NORTH);
    }

    ray = bitboardShift(cur, SOUTH);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, SOUTH);
    }

    ray = bitboardShift(cur, WEST);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, WEST);
    }

    ray = bitboardShift(cur, EAST);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, EAST);
    }

    return attacks;
}

Bitboard bishopAttacksForBlockers(Bitboard blockers, int sq) {

    Bitboard attacks = 0ULL;
    Bitboard cur = 0ULL;
    Bitboard ray = 0ULL;
    setSq(cur, sq);

    ray = bitboardShift(cur, NORTH_WEST);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, NORTH_WEST);
    }

    ray = bitboardShift(cur, SOUTH_WEST);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, SOUTH_WEST);
    }

    ray = bitboardShift(cur, NORTH_EAST);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, NORTH_EAST);
    }

    ray = bitboardShift(cur, SOUTH_EAST);
    while (ray != 0ULL)
    {

        attacks |= ray;
        if(ray & blockers) {
            break;
        }

        ray = bitboardShift(ray, SOUTH_EAST);
    }

    return attacks;
}