/*
 * This file contains implementations of:
 *   - xoshiro256**
 *   - SplitMix64
 *
 * Original authors:
 *   David Blackman and Sebastiano Vigna (xoshiro256**)
 *   Sebastiano Vigna (SplitMix64)
 *
 * Sources:
 *   https://prng.di.unimi.it/xoshiro256starstar.c
 *   https://prng.di.unimi.it/splitmix64.c
 *
 * Both algorithms are released under CC0 1.0 (Public Domain).
 * See https://creativecommons.org/publicdomain/zero/1.0/
 *
 * Modifications:
 *   - integration into PRNG struct
 *   - added a sparse random generator for magic numbers
 */

#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

#define PRNG_SEED_MAGIC 0x533DULL
#define PRNG_SEED_ZOBRIST 0x533DULL

typedef struct {
    uint64_t s[4];
} PRNG;

static inline uint64_t rotl64(uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static inline uint64_t splitmix64_seed(uint64_t *seed) {
    uint64_t z = (*seed += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

static inline void seed(PRNG* state, uint64_t seed) {
    for (int i = 0; i < 4; ++i)
        state->s[i] = splitmix64_seed(&seed);
}

static inline uint64_t next(PRNG* state) {
    const uint64_t result = rotl64(state->s[1] * 5, 7) * 9;

    const uint64_t t = state->s[1] << 17;

    state->s[2] ^= state->s[0];
    state->s[3] ^= state->s[1];
    state->s[1] ^= state->s[2];
    state->s[0] ^= state->s[3];

    state->s[2] ^= t;

    state->s[3] = rotl64(state->s[3], 45);

    return result;
}

// ANDing 3 random numbers for sparse random numbers (better for magic numbers)
static inline uint64_t randomMagic(PRNG* state) {
    return next(state) & next(state) & next(state);
}

#endif