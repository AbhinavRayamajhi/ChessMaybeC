#include "MoveGen.h"

#include "Bitboard.h"
#include "Board.h"
#include "Magic.h"
#include "MaskGen.h"
#include "Move.h"
#include "Position.h"

static inline void pruneAttacks(Board* board, Bitboard* attacks, GenType genType) {

    if (genType == CAPTURES) {

        Bitboard enemy = board->occ[!board->sideToMove];
        *attacks &= enemy;
    }
    else if (genType == QUIETS) {
        
        Bitboard occ = board->occ[BOTH];
        *attacks &= ~occ;
    }
    else if (genType == EVASIONS) {

        *attacks &= board->checkMask;
    }
    else if (genType == NON_EVASIONS) {

        Bitboard friends = board->occ[board->sideToMove];
        *attacks &= ~friends;
    }
}

static inline void createNormals(MoveList* moveList, Board* board, Square start, Bitboard attacks, GenType genType) {

    pruneAttacks(board, &attacks, genType);
    
    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
    }
}

static inline void createPromotions(MoveList* moveList, Board* board, Square start, Bitboard attacks, GenType genType, PushOrAttack pushOrAttack) {

    pruneAttacks(board, &attacks, genType);

    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        if (pushOrAttack == ATTACK) {

            // promotion piece is piece - 1 because for move packing we only have 2 bits for promotion pieces
            addMove(moveList, create(start, target, QUEEN - 1, PROMOTION));
            addMove(moveList, create(start, target, ROOK - 1, PROMOTION));
            addMove(moveList, create(start, target, KNIGHT - 1, PROMOTION));
            addMove(moveList, create(start, target, BISHOP - 1, PROMOTION));
        }
        // Queen promotions are so good that we want to evaluate them with captures even if not an attacking move
        else if (pushOrAttack == PUSH) {

            if (genType != QUIETS) {
                addMove(moveList, create(start, target, QUEEN - 1, PROMOTION));
            }
            if (genType != CAPTURES) {
                addMove(moveList, create(start, target, ROOK - 1, PROMOTION));
                addMove(moveList, create(start, target, KNIGHT - 1, PROMOTION));
                addMove(moveList, create(start, target, BISHOP - 1, PROMOTION));
            }
        }
    }
}

static inline void createPawnPushes(MoveList* moveList, Board* board, Bitboard attacks, PawnPushType pushType) {

    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        Square start;
        if (pushType == DOUBLE) start = target - (board->sideToMove == WHITE ? 16 : -16);
        else if (pushType == SINGLE) start = target - (board->sideToMove == WHITE ? 8 : -8);

        addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
    }
}

void enumeratePawnMoves(MoveList* moveList, Board* board, GenType genType) {

    Color side = board->sideToMove;

    Bitboard rank7 = side == WHITE ? RANK_7 : RANK_2;
    Bitboard pawns = board->pieces[side][PAWN];

    Bitboard pawnsOn7 = pawns & rank7;
    Bitboard pawnsNotOn7 = pawns & ~rank7;

    // promotions
    while (pawnsOn7) {

        Square start = getLSB(pawnsOn7);
        clearLSB(pawnsOn7);

        Bitboard attacks = pawnAttackTable[side][start] & board->occ[!side];
        createPromotions(moveList, board, start, attacks, genType, ATTACK);

        Bitboard pushTarget = boardFromSq(start + (side == WHITE ? 8 : -8)) & ~board->occ[BOTH];
        createPromotions(moveList, board, start, pushTarget, genType, PUSH);
    }
    
    // bulk pushes
    if (genType != CAPTURES) {

        Bitboard singlePush = singlePawnPush(pawnsNotOn7, side, board->occ[BOTH]);
        Bitboard doublePush = doublePawnPush(pawnsNotOn7, side, board->occ[BOTH]);

        pruneAttacks(board, &singlePush, genType);
        pruneAttacks(board, &doublePush, genType);

        createPawnPushes(moveList, board, singlePush, SINGLE);
        createPawnPushes(moveList, board, doublePush, DOUBLE);
    }

    // regular attacks
    while (pawnsNotOn7) {

        Square start = getLSB(pawnsNotOn7);
        clearLSB(pawnsNotOn7);

        Bitboard attacks = pawnAttackTable[side][start] & board->occ[!side];
        createNormals(moveList, board, start, attacks, genType);
    }

    // en passant
    if (board->enPassantSq != NONE) {

        Square target = board->enPassantSq;
        Bitboard attacks = pawns & pawnAttackTable[!side][target]; // attacks from ep sq anded with our pawns

        while (attacks) {

            Square start = getLSB(attacks);
            clearLSB(attacks);

            addMove(moveList, create(start, target, NO_PROMOTION_PIECE, EN_PASSANT));
        }
    }
}

void enumerateKingMoves(MoveList* moveList, Board* board, GenType genType) {

    Bitboard king = board->pieces[board->sideToMove][KING];

    Square start = getLSB(king);
    Bitboard attacks = kingTable[start];

    if (genType == CAPTURES) {
        Bitboard enemies = board->occ[!board->sideToMove];
        attacks &= enemies;
    }
    else {
        Bitboard friends = board->occ[board->sideToMove];
        attacks &= ~friends;
    }

    // remove king so that x-ray moves don't sneak in
    board->occ[BOTH] ^= king;
    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        // only add move if target not attacked
        if (!getSquareAttackers(board, target, board->sideToMove)) {
            addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }
    // put king back
    board->occ[BOTH] ^= king;
}

void enumerateCastlings(MoveList* moveList, Board* board) {

    if (board->sideToMove == WHITE && (board->castlingRight & WHITE_CASTLING)) {

        CastlingRights cr = getCR(board, WHITE);

        if (cr & WHITE_KINGSIDE) {
            addMove(moveList, create(E1, G1, NO_PROMOTION_PIECE, CASTLING));
        }
        if (cr & WHITE_QUEENSIDE) {
            addMove(moveList, create(E1, C1, NO_PROMOTION_PIECE, CASTLING));
        }
    }
    else if(board->sideToMove == BLACK && (board->castlingRight & BLACK_CASTLING)) {

        CastlingRights cr = getCR(board, BLACK);

        if (cr & BLACK_KINGSIDE) {
            addMove(moveList, create(E8, G8, NO_PROMOTION_PIECE, CASTLING));
        }
        if (cr & BLACK_QUEENSIDE) {
            addMove(moveList, create(E8, C8, NO_PROMOTION_PIECE, CASTLING));
        }
    }
}

void enumerateKnightMoves(MoveList* moveList, Board* board, GenType genType) {

    Bitboard knights = board->pieces[board->sideToMove][KNIGHT];

    while(knights) {

        Square start = getLSB(knights);
        clearLSB(knights);

        Bitboard attacks = knightTable[start];
        createNormals(moveList, board, start, attacks, genType);
    }
}

void enumerateRookMoves(MoveList* moveList, Board* board, GenType genType) {

    Bitboard rooks = board->pieces[board->sideToMove][ROOK];

    while(rooks) {

        Square start = getLSB(rooks);
        clearLSB(rooks);

        Bitboard attacks = getRookAttacks(board->occ[BOTH], start);
        createNormals(moveList, board, start, attacks, genType);
    }
}

void enumerateBishopMoves(MoveList* moveList, Board* board, GenType genType) {
    
    Bitboard bishops = board->pieces[board->sideToMove][BISHOP];

    while(bishops) {

        Square start = getLSB(bishops);
        clearLSB(bishops);

        Bitboard attacks = getBishopAttacks(board->occ[BOTH], start);
        createNormals(moveList, board, start, attacks, genType);
    }
}

void enumerateQueenMoves(MoveList* moveList, Board* board, GenType genType) {
    
    Bitboard queens = board->pieces[board->sideToMove][QUEEN];
    
    while (queens) {

        Square start = getLSB(queens);
        clearLSB(queens);

        Bitboard attacks = (getRookAttacks(board->occ[BOTH], start) | getBishopAttacks(board->occ[BOTH], start));
        createNormals(moveList, board, start, attacks, genType);
    }
}

static int legalCheck(Board* board, Move move) {

    Square start = getStartSq(move);

    // pinned pieces can only move in ray between attacker and king
    if (board->pinned & boardFromSq(start)) {

        Square target = getTargetSq(move);
        Square pinnerSq = board->pinners[start];
        Square kSq = getLSB(board->pieces[board->sideToMove][KING]);

        // pinned piece has to move between pinner and king (including pinner for capture)
        if (!(boardFromSq(target) & (rays[kSq][pinnerSq] | boardFromSq(pinnerSq)))) {
            return 0;
        }
    }
    if (getMoveType(move) == EN_PASSANT) {

        Board tempBoard = *board;
        Color side = tempBoard.sideToMove;
        Square target = getTargetSq(move);
        Square epPawnSq = target + (side == WHITE ? -8 : 8);
        Square kSq = getLSB(board->pieces[side][KING]);

        // simulate en passant on this temp board
        tempBoard.pieces[side][PAWN] ^= boardFromSq(start) | boardFromSq(target);
        tempBoard.pieces[!side][PAWN] ^= boardFromSq(epPawnSq);
        tempBoard.occ[BOTH] ^= boardFromSq(start) | boardFromSq(epPawnSq) | boardFromSq(target);

        // if in check after move, dont add move to movelist
        if (getSquareAttackers(&tempBoard, kSq, side)) {
            return 0;
        }
    }

    return 1;
}
// assumes king is not in check
void generateCaptures(MoveList* moveList, Board* board) {

    updateCheckInfo(board);
    MoveList temp = { .end = 0 };

    enumeratePawnMoves(&temp, board, CAPTURES);
    enumerateKnightMoves(&temp, board, CAPTURES);
    enumerateBishopMoves(&temp, board, CAPTURES);
    enumerateRookMoves(&temp, board, CAPTURES);
    enumerateQueenMoves(&temp, board, CAPTURES);
    enumerateKingMoves(&temp, board, CAPTURES);

    for (int i = 0; i < temp.end; ++i) {

        if (!legalCheck(board, temp.moveArray[i])) continue;

        moveList->moveArray[moveList->end] = temp.moveArray[i];
        ++moveList->end;
    }
}

void generateLegalMoves(MoveList* moveList, Board* board) {

    updateCheckInfo(board);
    MoveList temp = { .end = 0 };

    if (popCount(board->checkers) == 1) {

        enumeratePawnMoves(&temp, board, EVASIONS);
        enumerateKnightMoves(&temp, board, EVASIONS);
        enumerateBishopMoves(&temp, board, EVASIONS);
        enumerateRookMoves(&temp, board, EVASIONS);
        enumerateQueenMoves(&temp, board, EVASIONS);
    }
    else if (popCount(board->checkers) == 0) {
        
        enumeratePawnMoves(&temp, board, NON_EVASIONS);
        enumerateKnightMoves(&temp, board, NON_EVASIONS);
        enumerateBishopMoves(&temp, board, NON_EVASIONS);
        enumerateRookMoves(&temp, board, NON_EVASIONS);
        enumerateQueenMoves(&temp, board, NON_EVASIONS);
        enumerateCastlings(&temp, board);
    }
    
    enumerateKingMoves(&temp, board, ALL);

    // need legality check for en passant and pinned piece moves
    for (int i = 0; i < temp.end; ++i) {

        if (!legalCheck(board, temp.moveArray[i])) continue;

        moveList->moveArray[moveList->end++] = temp.moveArray[i];
    }
}