#include "Board.h"

#include "Bitboard.h"
#include "MaskGen.h"
#include "Magic.h"
#include "Helpers.h"
#include "PRNG.h"
#include "Zobrist.h"

#include <assert.h>

// runs when getting board from startpos or fen to create a piece & square mapping list
static void createPieceSqs(Board* board) {

    for (Square sq = A1; sq != NONE; ++sq) {

        for (Piece p = PAWN; p != PIECE_COUNT; ++p) {
            for (Color c = WHITE; c != BOTH; ++c) {
                if (getSq(board->pieces[c][p], sq)) {
                    board->pieceSqs[sq] = makeColored(c, p);
                    break;
                }
            }
        }
    }
}

// to get initial zobrist key from startpos or fen
static void createZKey(Board* board) {

    board->zobristKey = 0;

    for (Square sq = A1; sq < SQ_COUNT; ++sq) {
        
        if (board->pieceSqs[sq] != C_NO_PIECE)
            board->zobristKey  ^= ZOBRIST.pieceRandoms[board->pieceSqs[sq]][sq];
    }

    if (board->sideToMove == BLACK)
        board->zobristKey  ^= ZOBRIST.side;
    
    board->zobristKey ^= ZOBRIST.castlingRights[board->castlingRight];
    
    if (board->enPassantSq != NONE)
        board->zobristKey  ^= ZOBRIST.enPassantFile[board->enPassantSq % 8];

}

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
    board.pieces[BLACK][KING] = board.pieces[WHITE][KING] << 56;

    board.sideToMove = WHITE;
    board.castlingRight = 0b1111;
    board.enPassantSq = NONE;

    updateOcc(&board);
    createPieceSqs(&board);
    createZKey(&board);

    for (Square sq = A1; sq != NONE; ++sq) {

        board.pinners[sq] = NONE;
    }

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

    for (Square sq = A1; sq != NONE; ++sq) {
        board.pieceSqs[sq] = C_NO_PIECE;
        board.pinners[sq] = NONE;
    }

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
    createPieceSqs(&board);
    createZKey(&board);

    updateCheckInfo(&board);
    return board;
}

void moveP(Board* board, Square start, Square target, Piece p, Color side) {

    Bitboard movement = boardFromSq(start) | boardFromSq(target);
    board->pieces[side][p] ^= movement;
    board->occ[side] ^= movement;
    board->occ[BOTH] ^= movement;
}

void updateOcc(Board* board) {
    board->occ[WHITE] = board->occ[BLACK] = 0ULL;

    for (int p = 0; p < PIECE_COUNT; ++p) {
        board->occ[WHITE] |= board->pieces[WHITE][p];
        board->occ[BLACK] |= board->pieces[BLACK][p];
    }

    board->occ[BOTH] = board->occ[WHITE] | board->occ[BLACK];
}

void updateCombOcc(Board* board) {
    
    board->occ[BOTH] = board->occ[WHITE] | board->occ[BLACK];
}

void updateCheckInfo(Board* board) {

    Color side = board->sideToMove;
    Square kSq = getLSB(board->pieces[side][KING]);
    Bitboard enemySliders = board->pieces[!side][QUEEN] | board->pieces[!side][ROOK] | board->pieces[!side][BISHOP];

    for (Square sq = 0; sq != NONE; ++sq) {

        board->pinners[sq] = NONE;
    }
    board->checkers = getSquareAttackers(board, kSq, side);
    board->pinned = EMPTY_BOARD;
    board->checkMask = EMPTY_BOARD;

    // calculate rook and bishop attacks from ksq considering only enemy pieces
    Bitboard rookAtt = getRookAttacks(board->occ[!side], kSq);
    rookAtt &= board->pieces[!side][ROOK] | board->pieces[!side][QUEEN];

    Bitboard bishopAtt = getBishopAttacks(board->occ[!side], kSq);
    bishopAtt &= board->pieces[!side][BISHOP] | board->pieces[!side][QUEEN];

    Bitboard pinners = rookAtt | bishopAtt;

    // see if there's a pinned piece
    while (pinners) {

        Square pinSq = getLSB(pinners);
        clearLSB(pinners);

        Bitboard maybePinned = rays[kSq][pinSq] & board->occ[side];
        // if only one self piece between king and pinner, pinned
        if (popCount(maybePinned) == 1) {

            board->pinned |= maybePinned;
            board->pinners[getLSB(maybePinned)] = pinSq;
        }
    }

    // no checkers no check mask
    if (!board->checkers) {

        board->checkMask = FULL_BOARD;
    }
    // single check
    else if (popCount(board->checkers) == 1) {

        Square checkerSq = getLSB(board->checkers);
        Bitboard mask = EMPTY_BOARD;
        setSq(mask, checkerSq);

        // to allow blocking moves for slider checks, knight attacks will not trigger this so only choice is to capture or move
        if (mask & enemySliders)
            mask |= rays[kSq][checkerSq];
        
        board->checkMask = mask;
    }
    // multiple check only king move so checkmask will be empty
}

CastlingRights getCR(Board* board, Color side) {

    Bitboard occ = board->occ[BOTH];
    CastlingRights res = board->castlingRight;

    if (side == WHITE) {

        if (getSquareAttackers(board, F1, WHITE) || getSquareAttackers(board, G1, WHITE)
            || getSq(occ, F1) || getSq(occ, G1)) {
            res &= ~WHITE_KINGSIDE;
        }
        if (getSquareAttackers(board, D1, WHITE) || getSquareAttackers(board, C1, WHITE)
            || getSq(occ, D1) || getSq(occ, C1) || getSq(occ, B1)) {
            res &= ~WHITE_QUEENSIDE;
        }
    }
    else {

        if (getSquareAttackers(board, F8, BLACK) || getSquareAttackers(board, G8, BLACK)
            || getSq(occ, F8) || getSq(occ, G8)) {
            res &= ~BLACK_KINGSIDE;
        }
        if (getSquareAttackers(board, D8, BLACK) || getSquareAttackers(board, C8, BLACK)
            || getSq(occ, D8) || getSq(occ, C8) || getSq(occ, B8)) {
            res &= ~BLACK_QUEENSIDE;
        }
    }
    
    return res;
}

Bitboard getSquareAttackers(Board* board, Square sq, Color side) {

    assert(sq < 64);
    Bitboard attackers = 0ULL;

    attackers |= board->pieces[!side][PAWN] & pawnAttackTable[side][sq];
    attackers |= board->pieces[!side][KNIGHT] & knightTable[sq];
    attackers |= (board->pieces[!side][BISHOP] | board->pieces[!side][QUEEN]) & getBishopAttacks(board->occ[BOTH], sq);
    attackers |= (board->pieces[!side][ROOK] | board->pieces[!side][QUEEN]) & getRookAttacks(board->occ[BOTH], sq);
    attackers |= board->pieces[!side][KING] & kingTable[sq];

    return attackers;
}

void printBoard(Board* board) {

    for (int rank = 7; rank >= 0; --rank) {

        printf("%d   ", rank + 1);
        for (int file = 0; file < 8; ++file) {

            Square sq = rank * 8 + file;
            for (Piece p = PAWN; p <= PIECE_COUNT; ++p) {

                if (p == PIECE_COUNT) {
                    printf("%c ", '.');
                    break;
                }
                if (getSq(board->pieces[WHITE][p], sq)) {
                    printf("%c ", PIECES[p]);;
                    break;
                }
                if (getSq(board->pieces[BLACK][p], sq)) {
                    printf("%c ", PIECES[PIECE_COUNT + p]);
                    break;
                }
            }
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
}