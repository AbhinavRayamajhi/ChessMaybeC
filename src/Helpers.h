#include "Types.h"

static inline char rankFromSquare(Square sq) {

    return sq / 8 + '1';
}

static inline char fileFromSquare(Square sq) {

    return FILE_MAP[sq % 8];
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