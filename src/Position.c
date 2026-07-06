#include "Position.h"

#include <assert.h>
#include "Magic.h"
#include "MaskGen.h"

void makeMove(Board* board, History* history, Move move) {

    Square start = getStartSq(move);
    Square target = getTargetSq(move);
    Color side = board->sideToMove;

    history->prevMove = move;
    history->captured = NO_PIECE;
    history->prevEnPassantSq = board->enPassantSq;
    history->prevCastlingRights = board->castlingRight;
    history->prevHalfMoveClock = board->halfMoveClock;

    // reset board enPassant sq
    board->enPassantSq = NONE;
    
    // increment half move clock, will set back to 0 if pawn move or capture later
    board->halfMoveClock++;

    Piece startPiece = pieceOf(board->pieceSqs[start]);
    history->moved = startPiece;

     // move piece
    board->pieces[side][startPiece] ^= boardFromSq(start) | boardFromSq(target);
    board->occ[side] ^= boardFromSq(start) | boardFromSq(target);

    // remove if target has piece
    ColoredPiece cTargetPiece = board->pieceSqs[target];
    Piece targetPiece = cTargetPiece == C_NO_PIECE ? NO_PIECE : pieceOf(cTargetPiece);

    if (targetPiece != NO_PIECE) {
        history->captured = targetPiece;
        // reset half move clock since capture
        board->halfMoveClock = 0;
        // delete captured piece
        board->pieces[!side][targetPiece] ^= boardFromSq(target);
        board->occ[!side] ^= boardFromSq(target);
    }

    // update piece sq list
    board->pieceSqs[start] = C_NO_PIECE;
    board->pieceSqs[target] = makeColored(side, startPiece);

    if (startPiece == PAWN) {

        // for double push, we can xor start and target to find if change is 16
        // rank 2 -> rank 4 : 001xxx -> 011xxx and rank 7 -> rank 5 : 110xxx -> 100xxx
        // both xor's result is 010000 = 16
        if ((start ^ target) == 16) {
            
            board->enPassantSq = target - (side == WHITE ? 8 : -8);
        }
        board->halfMoveClock = 0;
    }

    // king move cancels all castling 
    if (startPiece == KING) {

        if (side == WHITE) board->castlingRight &= ~WHITE_CASTLING;
        else board->castlingRight &= ~BLACK_CASTLING;
    }

    // if rook moves from h1, h8, a1, a8 or rook captured castling rights is revoked, 
    if (startPiece == ROOK || targetPiece == ROOK) {

        if (start == H1 || target == H1) {
            board->castlingRight &= ~WHITE_KINGSIDE;
        }
        else if (start == H8 || target == H8) {
            board->castlingRight &= ~BLACK_KINGSIDE;
        }
        else if (start == A1 || target == A1) {
            board->castlingRight &= ~WHITE_QUEENSIDE;
        }
        else if (start == A8 || target == A8) {
            board->castlingRight &= ~BLACK_QUEENSIDE;
        }
    }

    if (getMoveType(move) == PROMOTION) {

        // remove moved pawn
        Piece promoted = getPromotionPiece(move);
        popSq(board->pieces[side][PAWN], target);
        setSq(board->pieces[side][promoted], target);

        board->pieceSqs[target] = makeColored(side, promoted);
    }
    else if (getMoveType(move) == EN_PASSANT) {

        Square epPawnSq = target + (side == WHITE ? -8 : 8);
        board->pieces[!side][PAWN] ^= boardFromSq(epPawnSq);
        board->occ[!side] ^= boardFromSq(epPawnSq);

        board->pieceSqs[epPawnSq] = C_NO_PIECE;
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

        board->pieces[side][ROOK] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);
        board->occ[side] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);

        board->pieceSqs[rookFrom] = C_NO_PIECE;
        board->pieceSqs[rookTo] = makeColored(side, ROOK);
    }    

    board->sideToMove ^= 1;
    updateCombOcc(board);
}

void unmakeMove(Board* board, History* history) {

    Square start = getStartSq(history->prevMove);
    Square target = getTargetSq(history->prevMove);
    Color side = board->sideToMove;

    board->enPassantSq = history->prevEnPassantSq;
    board->castlingRight = history->prevCastlingRights;
    board->halfMoveClock = history->prevHalfMoveClock;

    board->pieceSqs[target] = C_NO_PIECE;

    // update other side's occ
    board->occ[!side] ^= boardFromSq(start) | boardFromSq(target);

    if (history->captured != NO_PIECE) {

        if (getMoveType(history->prevMove) != EN_PASSANT) {
            board->pieces[side][history->captured] ^= boardFromSq(target);
            board->occ[side] ^= boardFromSq(target);
            board->pieceSqs[target] = makeColored(side, history->captured);
        }
        else {
            Square epPawnSq = target + (side == WHITE ? 8 : -8);
            board->pieces[side][PAWN] ^= boardFromSq(epPawnSq);
            board->occ[side] ^= boardFromSq(epPawnSq);
            board->pieceSqs[epPawnSq] = makeColored(side, PAWN);
        }
    }

    if (getMoveType(history->prevMove) == PROMOTION) {

        Piece promoted = getPromotionPiece(history->prevMove);
        board->pieces[!side][promoted] ^= boardFromSq(target);
        board->pieces[!side][PAWN] ^= boardFromSq(start);

        board->pieceSqs[start] = makeColored(!side, PAWN);
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

        board->pieces[!side][ROOK] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);
        board->occ[!side] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);
        board->pieces[!side][KING] ^= boardFromSq(start) | boardFromSq(target);

        board->pieceSqs[rookTo] = C_NO_PIECE;
        board->pieceSqs[rookFrom] = makeColored(!side, ROOK);
        board->pieceSqs[start] = makeColored(!side, KING);
    }
    else {
        board->pieces[!side][history->moved] ^= boardFromSq(start)| boardFromSq(target);
        board->pieceSqs[start] = makeColored(!side, history->moved);
    }

    board->sideToMove ^= 1;
    updateCombOcc(board);
}