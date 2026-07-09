#include "Position.h"

#include <assert.h>
#include "Magic.h"
#include "MaskGen.h"
#include "Zobrist.h"

void makeMove(Board* board, History* history, Move move) {

    Square startSq = getStartSq(move);
    Square targetSq = getTargetSq(move);
    Color side = board->sideToMove;

    history->prevMove = move;
    history->captured = NO_PIECE;
    history->prevEnPassantSq = board->enPassantSq;
    history->prevCastlingRights = board->castlingRight;
    history->prevHalfMoveClock = board->halfMoveClock;

    // reset board enPassant sq
    if (board->enPassantSq != NONE) board->zobristKey ^= ZOBRIST.enPassantFile[fileOf(board->enPassantSq)];
    board->enPassantSq = NONE;
    
    // increment half move clock, will set back to 0 if pawn move or capture later
    board->halfMoveClock++;

    ColoredPiece cStartPiece = board->pieceSqs[startSq];
    Piece startPiece = pieceOf(cStartPiece);
    history->moved = startPiece;

     // move piece
    board->pieces[side][startPiece] ^= boardFromSq(startSq) | boardFromSq(targetSq);
    board->occ[side] ^= boardFromSq(startSq) | boardFromSq(targetSq);

    board->zobristKey ^= ZOBRIST.pieceRandoms[cStartPiece][startSq];
    board->zobristKey ^= ZOBRIST.pieceRandoms[cStartPiece][targetSq];

    // remove if target has piece
    ColoredPiece cTargetPiece = board->pieceSqs[targetSq];
    Piece targetPiece = cTargetPiece == C_NO_PIECE ? NO_PIECE : pieceOf(cTargetPiece);

    if (targetPiece != NO_PIECE) {

        history->captured = targetPiece;

        // delete captured piece
        board->pieces[!side][targetPiece] ^= boardFromSq(targetSq);
        board->occ[!side] ^= boardFromSq(targetSq);

        board->zobristKey ^= ZOBRIST.pieceRandoms[cTargetPiece][targetSq];
        
        // reset half move clock since capture
        board->halfMoveClock = 0;
    }

    // update piece sq list
    board->pieceSqs[startSq] = C_NO_PIECE;
    board->pieceSqs[targetSq] = makeColored(side, startPiece);

    if (startPiece == PAWN) {

        // for double push, we can xor start and target to find if change is 16
        // rank 2 -> rank 4 : 001xxx -> 011xxx and rank 7 -> rank 5 : 110xxx -> 100xxx
        // both xor's result is 010000 = 16
        if ((startSq ^ targetSq) == 16) {

            Square enPassantSq = targetSq - (side == WHITE ? 8 : -8);

            board->enPassantSq = enPassantSq;
            board->zobristKey ^= ZOBRIST.enPassantFile[fileOf(enPassantSq)];
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

        if (startSq == H1 || targetSq == H1) {
            board->castlingRight &= ~WHITE_KINGSIDE;
        }
        else if (startSq == H8 || targetSq == H8) {
            board->castlingRight &= ~BLACK_KINGSIDE;
        }
        else if (startSq == A1 || targetSq == A1) {
            board->castlingRight &= ~WHITE_QUEENSIDE;
        }
        else if (startSq == A8 || targetSq == A8) {
            board->castlingRight &= ~BLACK_QUEENSIDE;
        }
    }

    // if castling right changed, have to update zobrist key
    if (history->prevCastlingRights != board->castlingRight) {

        board->zobristKey ^= ZOBRIST.castlingRights[history->prevCastlingRights];
        board->zobristKey ^= ZOBRIST.castlingRights[board->castlingRight];
    }

    if (getMoveType(move) == PROMOTION) {

        // remove moved pawn
        Piece promoted = getPromotionPiece(move);

        popSq(board->pieces[side][PAWN], targetSq);
        setSq(board->pieces[side][promoted], targetSq);

        ColoredPiece coloredPromoted = makeColored(side, promoted);

        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(side, PAWN)][targetSq];
        board->zobristKey ^= ZOBRIST.pieceRandoms[coloredPromoted][targetSq];

        board->pieceSqs[targetSq] = coloredPromoted;
    }
    else if (getMoveType(move) == EN_PASSANT) {

        // remove enpassanted pawn
        Square epPawnSq = targetSq + (side == WHITE ? -8 : 8);
        board->pieces[!side][PAWN] ^= boardFromSq(epPawnSq);
        board->occ[!side] ^= boardFromSq(epPawnSq);

        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, PAWN)][epPawnSq];

        board->pieceSqs[epPawnSq] = C_NO_PIECE;
        history->captured = PAWN;
    }
    else if (getMoveType(move) == CASTLING) {

        Square rookFrom, rookTo;
        if (targetSq == G1) {
            rookFrom = H1;
            rookTo = F1;
        }
        else if (targetSq == G8) {
            rookFrom = H8;
            rookTo = F8;
        }
        else if (targetSq == C1) {
            rookFrom = A1;
            rookTo = D1;
        }
        else {
            rookFrom = A8;
            rookTo = D8;
        }

        board->pieces[side][ROOK] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);
        board->occ[side] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);

        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(side, ROOK)][rookFrom];
        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(side, ROOK)][rookTo];

        board->pieceSqs[rookFrom] = C_NO_PIECE;
        board->pieceSqs[rookTo] = makeColored(side, ROOK);
    }    

    board->zobristKey ^= ZOBRIST.side;
    board->sideToMove ^= 1;
    updateCombOcc(board);
}

void unmakeMove(Board* board, History* history) {

    Square startSq = getStartSq(history->prevMove);
    Square targetSq = getTargetSq(history->prevMove);
    Color side = board->sideToMove;

    // reset zobrist key for enpassant and castling rights
    if (board->enPassantSq != NONE) {
        board->zobristKey ^= ZOBRIST.enPassantFile[fileOf(board->enPassantSq)];
    }

    if (history->prevEnPassantSq != NONE) {
        board->zobristKey ^= ZOBRIST.enPassantFile[fileOf(history->prevEnPassantSq)];
    }

    // if castling right changed, have to update zobrist key
    if (history->prevCastlingRights != board->castlingRight) {

        board->zobristKey ^= ZOBRIST.castlingRights[history->prevCastlingRights];
        board->zobristKey ^= ZOBRIST.castlingRights[board->castlingRight];
    }

    board->enPassantSq = history->prevEnPassantSq;
    board->castlingRight = history->prevCastlingRights;
    board->halfMoveClock = history->prevHalfMoveClock;

    board->pieceSqs[targetSq] = C_NO_PIECE;

    // update other side's occ
    board->occ[!side] ^= boardFromSq(startSq) | boardFromSq(targetSq);

    if (history->captured != NO_PIECE) {

        if (getMoveType(history->prevMove) != EN_PASSANT) {

            board->pieces[side][history->captured] ^= boardFromSq(targetSq);
            board->occ[side] ^= boardFromSq(targetSq);

            board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(side, history->captured)][targetSq];

            board->pieceSqs[targetSq] = makeColored(side, history->captured);
        }
        else {

            Square epPawnSq = targetSq + (side == WHITE ? 8 : -8);

            board->pieces[side][PAWN] ^= boardFromSq(epPawnSq);
            board->occ[side] ^= boardFromSq(epPawnSq);

            board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(side, PAWN)][epPawnSq];

            board->pieceSqs[epPawnSq] = makeColored(side, PAWN);
        }
    }

    if (getMoveType(history->prevMove) == PROMOTION) {

        Piece promoted = getPromotionPiece(history->prevMove);

        board->pieces[!side][promoted] ^= boardFromSq(targetSq);
        board->pieces[!side][PAWN] ^= boardFromSq(startSq);

        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, promoted)][targetSq];
        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, PAWN)][startSq];

        board->pieceSqs[startSq] = makeColored(!side, PAWN);
    }
    else if (getMoveType(history->prevMove) == CASTLING) {

        Square rookFrom, rookTo;
        if (targetSq == G1) {
            rookFrom = H1;
            rookTo = F1;
        }
        else if (targetSq == G8) {
            rookFrom = H8;
            rookTo = F8;
        }
        else if (targetSq == C1) {
            rookFrom = A1;
            rookTo = D1;
        }
        else {
            rookFrom = A8;
            rookTo = D8;
        }

        board->pieces[!side][ROOK] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);
        board->occ[!side] ^= boardFromSq(rookFrom) | boardFromSq(rookTo);
        board->pieces[!side][KING] ^= boardFromSq(startSq) | boardFromSq(targetSq);

        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, ROOK)][rookFrom];
        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, ROOK)][rookTo];
        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, KING)][startSq];
        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, KING)][targetSq];

        board->pieceSqs[rookTo] = C_NO_PIECE;
        board->pieceSqs[rookFrom] = makeColored(!side, ROOK);
        board->pieceSqs[startSq] = makeColored(!side, KING);
    }
    else {

        board->pieces[!side][history->moved] ^= boardFromSq(startSq) | boardFromSq(targetSq);

        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, history->moved)][startSq];
        board->zobristKey ^= ZOBRIST.pieceRandoms[makeColored(!side, history->moved)][targetSq];

        board->pieceSqs[startSq] = makeColored(!side, history->moved);
    }

    board->zobristKey ^= ZOBRIST.side;
    board->sideToMove ^= 1;
    updateCombOcc(board);
}