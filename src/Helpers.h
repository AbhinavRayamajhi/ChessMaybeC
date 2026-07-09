#include "Types.h"

static inline Rank rankOf(Square sq) {

    return sq >> 3;
}

static inline File fileOf(Square sq) {

    return sq & 0b000111;
}

static inline char rankFromSquare(Square sq) {

    return rankOf(sq) + '1';
}

static inline char fileFromSquare(Square sq) {

    return fileOf(sq) + 'a';
}

static inline Color colorOf(ColoredPiece piece) {

    return (Color)(piece / PIECE_COUNT);
}

static inline Piece pieceOf(ColoredPiece piece) {

    return (Piece)(piece % PIECE_COUNT);
}

static inline ColoredPiece makeColored(Color side, Piece piece) {

    return (ColoredPiece)(side * PIECE_COUNT + piece);
}