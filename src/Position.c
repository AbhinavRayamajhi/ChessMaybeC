#include "Position.h"

#include <assert.h>

void makeMove(Board* board, History* history, Move move) {

    Square start = getStartSq(move);
    Square target = getTargetSq(move);

    history->prevMove = move;
    history->captured = NO_PIECE;
    history->prevEnPassantSq = board->enPassantSq;
    history->prevCastlingRights = board->castlingRight;
    history->prevHalfMoveClock = board->halfMoveClock;

    // reset board enPassant sq
    board->enPassantSq = NONE;
    
    // increment half move clock, will set back to 0 if pawn move or capture later
    board->halfMoveClock++;

    for (Piece piece = PAWN; piece < PIECE_COUNT; ++piece) {

        if (getSq(board->pieces[board->sideToMove][piece], start)) {

            history->moved = piece;

            // move piece
            board->pieces[board->sideToMove][piece] ^= (1ULL << start) | (1ULL << target);

            if (piece == PAWN) {
                // set en passant sq if double push
                if (start + 16 == target) board->enPassantSq = start + 8;
                else if (start - 16 == target) board->enPassantSq = start - 8;
                // reset half move clock since pawn move
                board->halfMoveClock = 0;
            }

            // king move cancels all castling
            if (piece == KING) {

                if (board->sideToMove == WHITE) board->castlingRight &= ~WHITE_CASTLING;
                else board->castlingRight &= ~BLACK_CASTLING;
            }
        }
        
        if (getSq(board->pieces[!board->sideToMove][piece], target)) {

            history->captured = piece;
            // reset half move clock since capture
            board->halfMoveClock = 0;
            // delete captured piece
            board->pieces[!board->sideToMove][piece] ^= (1ULL << target);
        }
    }

    if (getMoveType(move) == PROMOTION) {

        // remove moved pawn
        popSq(board->pieces[board->sideToMove][PAWN], target);
        setSq(board->pieces[board->sideToMove][getPromotionPiece(move)], target);
    }
    else if (getMoveType(move) == EN_PASSANT) {

        board->pieces[!board->sideToMove][PAWN] ^= (1ULL << (target + (board->sideToMove ? 8 : -8)));
        history->captured = PAWN;
    }
    else if (getMoveType(move) == CASTLING) {

        Square rookFrom, rookTo;
        if (target == G1) {
            rookFrom = H1;
            rookTo = F1;
        }
        else if (target == G8) {
            rookFrom = H8;
            rookTo = F8;
        }
        else if (target == C1) {
            rookFrom = A1;
            rookTo = D1;
        }
        else {
            rookFrom = A8;
            rookTo = D8;
        }

        board->pieces[board->sideToMove][ROOK] ^= (1ULL << rookFrom) | (1ULL << rookTo);
    }

    // if anything moves from h1, h8, a1, a8, castling rights is revoked, checking if moved piece is rook could be faster, Need to check
    if (start == H1 || target == H1) {
        board->castlingRight &= ~WHITE_KINGSIDE;
    }
    else if (start == H8 || target == H8) {
        board->castlingRight &= ~BLACK_KINGSIDE;
    }
    else if (start == A1 || target == A1) {
        board->castlingRight &= ~WHITE_QUEENSIDE;
    }
    else if (start == A8 || target == H8) {
        board->castlingRight &= ~BLACK_QUEENSIDE;
    }

    board->sideToMove ^= 1;
    updateOcc(board);
    updateCheckInfo(board);
}

void unmakeMove(Board* board, History* history) {

    Square start = getStartSq(history->prevMove);
    Square target = getTargetSq(history->prevMove);
    
    board->enPassantSq = history->prevEnPassantSq;
    board->castlingRight = history->prevCastlingRights;
    board->halfMoveClock = history->prevHalfMoveClock;

    if (history->captured != NO_PIECE) {

        if (getMoveType(history->prevMove) != EN_PASSANT) {
            board->pieces[board->sideToMove][history->captured] ^= 1ULL << target;
        }
        else {
            board->pieces[board->sideToMove][PAWN] ^= (1ULL << (target + (board->sideToMove ? -8 : 8)));
        }
    }

    if (getMoveType(history->prevMove) == PROMOTION) {

        popSq(board->pieces[!board->sideToMove][getPromotionPiece(history->prevMove)], target);
        setSq(board->pieces[!board->sideToMove][PAWN], start);
    }
    else if (getMoveType(history->prevMove) == CASTLING) {

        Square rookFrom, rookTo;
        if (target == G1) {
            rookFrom = H1;
            rookTo = F1;
        }
        else if (target == G8) {
            rookFrom = H8;
            rookTo = F8;
        }
        else if (target == C1) {
            rookFrom = A1;
            rookTo = D1;
        }
        else {
            rookFrom = A8;
            rookTo = D8;
        }

        board->pieces[!board->sideToMove][ROOK] ^= (1ULL << rookFrom) | (1ULL << rookTo);
        board->pieces[!board->sideToMove][KING] ^= (1ULL << start) | (1ULL << target);
    }
    else {
        board->pieces[!board->sideToMove][history->moved] ^= (1ULL << start) | (1ULL << target);
    }

    board->sideToMove ^= 1;
    updateOcc(board);
}