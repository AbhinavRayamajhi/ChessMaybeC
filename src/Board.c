#include "Board.h"

#include "Bitboard.h"

Board getInitialBoard() {

    Board res;

    res.pieces[WHITE][PAWN]   = 0x000000000000FF00ULL;
    res.pieces[WHITE][KNIGHT] = 0x0000000000000042ULL;
    res.pieces[WHITE][BISHOP] = 0x0000000000000024ULL;
    res.pieces[WHITE][ROOK]   = 0x0000000000000081ULL;
    res.pieces[WHITE][QUEEN]  = 0x0000000000000008ULL;
    res.pieces[WHITE][KING]   = 0x0000000000000010ULL;

    res.pieces[BLACK][PAWN]   = res.pieces[WHITE][PAWN]   << 40;
    res.pieces[BLACK][KNIGHT] = res.pieces[WHITE][KNIGHT] << 56;
    res.pieces[BLACK][BISHOP] = res.pieces[WHITE][BISHOP] << 56;
    res.pieces[BLACK][ROOK]   = res.pieces[WHITE][ROOK]   << 56;
    res.pieces[BLACK][QUEEN]  = res.pieces[WHITE][QUEEN]  << 56;
    res.pieces[BLACK][KING]   = res.pieces[WHITE][KING]   << 56;

    res.occ[WHITE] = res.occ[BLACK] = res.occ[BOTH] = 0ULL;

    for (int p = 0; p < PIECE_COUNT; ++p) {
        res.occ[WHITE] |= res.pieces[WHITE][p];
        res.occ[BLACK] |= res.pieces[BLACK][p];
    }

    res.occ[BOTH] = res.occ[WHITE] | res.occ[BLACK];

    res.sideToMove = WHITE;
    res.castlingRight = 0b1111;
    res.enPassantSq = NONE;
    res.halfMoveClock = res.fullMoveClock = 0;

    return res;
}

Board getZeroBoard() {
    Board board;

    for (Piece p = 0; p < 6; ++p) {
        board.pieces[WHITE][p] = 0ULL;
        board.pieces[BLACK][p] = 0ULL;
    }
    updateOcc(&board);
    board.sideToMove = 0;
    board.castlingRight = 0;
    board.enPassantSq = NONE;
    board.halfMoveClock = 0;
    board.fullMoveClock = 0;

    return board;
}

Board getBoardFromFen(const char* FEN) {

    int rank = 7;
    int file = 0;

    Board board = getZeroBoard();

    while (*FEN != ' ') {

        Square sq = rank * 8 + file;
        switch (*FEN) {

        case '1': file++; break;
        case '2': file += 2; break;
        case '3': file += 3; break;
        case '4': file += 4; break;
        case '5': file += 5; break;
        case '6': file += 6; break;
        case '7': file += 7; break;
        case '8': file += 8; break;

        case 'p': setSq(board.pieces[BLACK][PAWN], sq); file++; break;
        case 'n': setSq(board.pieces[BLACK][KNIGHT], sq); file++; break;
        case 'b': setSq(board.pieces[BLACK][BISHOP], sq); file++; break;
        case 'r': setSq(board.pieces[BLACK][ROOK], sq); file++; break;
        case 'q': setSq(board.pieces[BLACK][QUEEN], sq); file++; break;
        case 'k': setSq(board.pieces[BLACK][KING], sq); file++; break;
            
        case 'P': setSq(board.pieces[WHITE][PAWN], sq); file++; break;
        case 'N': setSq(board.pieces[WHITE][KNIGHT], sq); file++; break;
        case 'B': setSq(board.pieces[WHITE][BISHOP], sq); file++; break;
        case 'R': setSq(board.pieces[WHITE][ROOK], sq); file++; break;
        case 'Q': setSq(board.pieces[WHITE][QUEEN], sq); file++; break;
        case 'K': setSq(board.pieces[WHITE][KING], sq); file++; break;

        default: break;
        }

        if (file == 8) {
            file = 0;
            rank--;
        }
        FEN++;
    }

    FEN++;
    board.sideToMove = *FEN == 'w' ? WHITE : BLACK;
    FEN += 2;

    while (*FEN != '-' && *FEN != ' ') {
        if (*FEN == 'K') board.castlingRight |= WHITE_KINGSIDE;
        else if (*FEN == 'Q') board.castlingRight |= WHITE_QUEENSIDE;
        else if (*FEN == 'k') board.castlingRight |= BLACK_KINGSIDE;
        else if (*FEN == 'q') board.castlingRight |= BLACK_QUEENSIDE;
        FEN++;
    }

    if (*FEN == '-') FEN++;
    FEN++;

    if (*FEN != '-') {
        file = *FEN - 'a';
        FEN++;
        rank = *FEN - '1';
        board.enPassantSq = rank * 8 + file;
    }

    FEN += 2;

    board.halfMoveClock = atoi(FEN);
    while (*FEN != ' ') FEN++;
    FEN++;
    board.fullMoveClock = atoi(FEN);

    updateOcc(&board);
    return board;
}

void updateOcc(Board* b) {
    b->occ[WHITE] = b->occ[BLACK] = b->occ[BOTH] = 0ULL;

    for (int p = 0; p < PIECE_COUNT; ++p) {
        b->occ[WHITE] |= b->pieces[WHITE][p];
        b->occ[BLACK] |= b->pieces[BLACK][p];
    }

    b->occ[BOTH] = b->occ[WHITE] | b->occ[BLACK];
}

