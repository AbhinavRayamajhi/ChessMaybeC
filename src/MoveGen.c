#include "MoveGen.h"

#include "Bitboard.h"
#include "Board.h"
#include "Magic.h"
#include "MaskGen.h"
#include "Move.h"
#include "Position.h"

#include <assert.h>

void pruneAttacks(Board* board, Bitboard* attacks, GenType genType) {

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

void createNormals(MoveList* moveList, Board* board, Square start, Bitboard attacks, GenType genType) {

    pruneAttacks(board, &attacks, genType);
    
    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);
        addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
    }
}

void createPromotions(MoveList* moveList, Board* board, Square start, Bitboard attacks, GenType genType, int isAttack) {

    pruneAttacks(board, &attacks, genType);

    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(target);

        if (isAttack) {

            addMove(moveList, create(start, target, QUEEN, PROMOTION));
            addMove(moveList, create(start, target, ROOK, PROMOTION));
            addMove(moveList, create(start, target, KNIGHT, PROMOTION));
            addMove(moveList, create(start, target, BISHOP, PROMOTION));
        }
        // Queen promotions are so good that we want to evaluate them with captures even if not an attacking move
        else if (!isAttack) {

            if (genType != QUIETS) {
                addMove(moveList, create(start, target, QUEEN, PROMOTION));
            }
            if (genType != CAPTURES) {
                addMove(moveList, create(start, target, ROOK, PROMOTION));
                addMove(moveList, create(start, target, KNIGHT, PROMOTION));
                addMove(moveList, create(start, target, BISHOP, PROMOTION));
            }
        }
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
        
        Bitboard pushTarget = boardFromSq(start + (side == WHITE ? 8 : -8));
        createPromotions(moveList, board, start, pushTarget, genType, 0);

        Bitboard attacks = pawnAttackTable[side * 64 + start];
        createPromotions(moveList, board, start, attacks, genType, 1);
    }
    // regular push and attacks
    while (pawnsNotOn7) {

        Square start = getLSB(pawnsNotOn7);
        clearLSB(pawnsNotOn7);
        Bitboard attacks = pawnAttackTable[side * 64 + start];

        createNormals(moveList, board, start, attacks, genType);
    }
    // en passant
    if (board->enPassantSq != NONE) {

        Square target = board->enPassantSq;
        Bitboard attacks = pawns & pawnAttackTable[!side * 64 + target]; // attacks from ep sq anded with our pawns

        while (attacks) {

            Square start = getLSB(attacks);
            clearLSB(attacks);

            addMove(moveList, create(start, target, NO_PROMOTION_PIECE, EN_PASSANT));
        }
    }
}

void enumerateKingMoves(MoveList* moveList, Board* board) {

    Bitboard king = board->pieces[board->sideToMove][KING];
    Bitboard friends = board->occ[board->sideToMove];

    Square start = getLSB(king);
    Bitboard attacks = kingTable[start] & ~friends;

    // need to figure out a way to prune king attacks to avoid checked sqs

    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        // only add move if target not attacked
        if (!getSquareAttackers(board, target, board->sideToMove)) {
            addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }
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
    while(queens) {

        Square start = getLSB(queens);
        clearLSB(queens);

        Bitboard attacks = (getRookAttacks(board->occ[BOTH], start) | getBishopAttacks(board->occ[BOTH], start));
        createNormals(moveList, board, start, attacks, genType);
    }
}

void generateLegalMoves(MoveList* moveList, Board* board) {

    updateCheckInfo(board);
    
    enumerateKingMoves(moveList, board);

    if (popCount(board->checkers) == 1) {

        enumeratePawnMoves(moveList, board, EVASIONS);
        enumerateBishopMoves(moveList, board, EVASIONS);
        enumerateKnightMoves(moveList, board, EVASIONS);
        enumerateRookMoves(moveList, board, EVASIONS);
        enumerateQueenMoves(moveList, board, EVASIONS);
    }
    else if (popCount(board->checkers) == 0) {
        
        enumeratePawnMoves(moveList, board, NON_EVASIONS);
        enumerateCastlings(moveList, board);
        enumerateBishopMoves(moveList, board, NON_EVASIONS);
        enumerateKnightMoves(moveList, board, NON_EVASIONS);
        enumerateRookMoves(moveList, board, NON_EVASIONS);
        enumerateQueenMoves(moveList, board, NON_EVASIONS);
    }

    // pinned pieces can only move in ray between attacker and king
    for (int i = 0; i < moveList->end; ++i) {

        Square start = getStartSq(moveList->moveArray[i]);
        if (board->pinned & boardFromSq(start)) {

            Square target = getTargetSq(moveList->moveArray[i]);
            Square pinnerSq = board->pinners[start];
            Square kSq = board->pieces[board->sideToMove][KING];

            if (!(boardFromSq(target) & rays[kSq][pinnerSq])) {
                removeMove(moveList, i);
                --i;
            }
        }
    }
}