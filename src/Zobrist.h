#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "Types.h"
#include "PRNG.h"

typedef struct {
    Key pieceRandoms[C_PIECE_COUNT][SQ_COUNT];
    Key side;
    Key castlingRights[16];
    Key enPassantFile[8];
} Zobrist;

static inline void initZobrist(Zobrist* zobrist) {

    PRNG state;
    seed(&state, PRNG_SEED_ZOBRIST);

    for (ColoredPiece p = W_PAWN; p < C_PIECE_COUNT; ++p) {
        for (Square sq = A1; sq != NONE; ++sq) {
            zobrist->pieceRandoms[p][sq] = next(&state);
        }
    }

    zobrist->side = next(&state);

    for (CastlingRights cr = NO_CASTLING; cr <= ALL_CASTLING; ++cr) {
        zobrist->castlingRights[cr] = next(&state);
    }

    for (int file = 0; file < 8; ++file) {
        zobrist->enPassantFile[file] = next(&state);
    }
}

extern Zobrist ZOBRIST;

#endif