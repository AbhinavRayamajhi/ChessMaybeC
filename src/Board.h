#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include "Types.h"

typedef struct {

    Bitboard pieces[COLOR_COUNT][PIECE_COUNT];
    Bitboard occ[COLOR_COUNT + 1];
    ColoredPiece pieceSqs[SQ_COUNT];
    Square pinners[SQ_COUNT];

    Key zobristKey;
    Color sideToMove;
    CastlingRights castlingRight;
    Square enPassantSq;

    Bitboard checkMask;
    Bitboard checkers;
    Bitboard pinned;

    int halfMoveClock;
    int fullMoveClock;

} Board;

Board getInitialBoard();
Board getBoardFromFen(const char* FEN);

void moveP(Board* board, Square start, Square target, Piece p, Color side);

void updateOcc(Board* b);
void updateCombOcc(Board* b);
void updateCheckInfo(Board* board);

CastlingRights getCR(Board* board, Color side);
Bitboard getSquareAttackers(Board* board, Square sq, Color side);
Bitboard isKingInCheck(Board* board);

void printBoard(Board* board);

#endif
