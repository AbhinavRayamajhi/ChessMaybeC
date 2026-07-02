#include "Board.h"

#include "Bitboard.h"

Board getInitialBoard() {

    Board board;

    board.pieces[WHITE][PAWN]   = 0x000000000000FF00ULL;
    board.pieces[WHITE][KNIGHT] = 0x0000000000000042ULL;
    board.pieces[WHITE][BISHOP] = 0x0000000000000024ULL;
    board.pieces[WHITE][ROOK]   = 0x0000000000000081ULL;
    board.pieces[WHITE][QUEEN]  = 0x0000000000000008ULL;
    board.pieces[WHITE][KING]   = 0x0000000000000010ULL;

    board.pieces[BLACK][PAWN]   = board.pieces[WHITE][PAWN]   << 40;
    board.pieces[BLACK][KNIGHT] = board.pieces[WHITE][KNIGHT] << 56;
    board.pieces[BLACK][BISHOP] = board.pieces[WHITE][BISHOP] << 56;
    board.pieces[BLACK][ROOK]   = board.pieces[WHITE][ROOK]   << 56;
    board.pieces[BLACK][QUEEN]  = board.pieces[WHITE][QUEEN]  << 56;
    board.pieces[BLACK][KING]   = board.pieces[WHITE][KING]   << 56;

    updateOcc(&board);

    board.sideToMove = WHITE;
    board.castlingRight = 0b1111;
    board.enPassantSq = NONE;

    board.checkMask = 0ULL;
    board.checkers = 0ULL;
    board.pinned = 0ULL;

    board.halfMoveClock = board.fullMoveClock = 0;

    return board;
}

Board getZeroBoard() {

    Board board;

    for (Piece p = 0; p < 6; ++p) {
        board.pieces[WHITE][p] = 0ULL;
        board.pieces[BLACK][p] = 0ULL;
    }
    board.occ[BOTH] = board.occ[WHITE] = board.occ[BLACK] = 0ULL;

    board.sideToMove = 0;
    board.castlingRight = 0;
    board.enPassantSq = NONE;

    board.checkMask = 0ULL;
    board.checkers = 0ULL;
    board.pinned = 0ULL;

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
    updateCheckInfo(&board);
    return board;
}

void updateOcc(Board* board) {
    board->occ[WHITE] = board->occ[BLACK] = 0ULL;

    for (int p = 0; p < PIECE_COUNT; ++p) {
        board->occ[WHITE] |= board->pieces[WHITE][p];
        board->occ[BLACK] |= board->pieces[BLACK][p];
    }

    board->occ[BOTH] = board->occ[WHITE] | board->occ[BLACK];
}

void updateCheckInfo(Board* board) {
    
}
