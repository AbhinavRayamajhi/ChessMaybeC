#include "MoveGen.h"

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

    pruneAttacks(board, attacks, genType);
    
    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);
        addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
    }
}

void createPromotions(MoveList* moveList, Board* board, Square start, Bitboard attacks, GenType genType, int isAttack) {

    pruneAttacks(board, attacks, genType);

    while (attacks) {

        Square target = getLSB(attacks);
        clearLS(target);

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

    Bitboard occ = board->pieces[BOTH];

    // promotions
    while (pawnsOn7) {

        Square start = getLSB(pawnsOn7);
        clearLSB(pawnsOn7);
        
        Bitboard pushTarget = 1ULL << (start + (side == WHITE ? 8 : -8));
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

    // need to figure out a way to prune king attacks

    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        addMove(moveList, create(start, target, NO_PROMOTION_PIECE, NORMAL));
    }
}

void enumerateCastlings(MoveList* moveList, Board* board) {
    
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

Bitboard getSquareAttackers(Board* board, Square sq) {

    Bitboard attackers = 0ULL;
    Bitboard self = 1ULL << sq;

    attackers |= board->pieces[board->sideToMove][PAWN] & (pawnLeftAttack(self, !board->sideToMove)
        | pawnRightAttack(self, !board->sideToMove));
    attackers |= board->pieces[board->sideToMove][KNIGHT] & knightTable[sq];
    attackers |= (board->pieces[board->sideToMove][BISHOP] | board->pieces[board->sideToMove][QUEEN])
        & getBishopAttacks(board->occ[BOTH], sq);
    attackers |= (board->pieces[board->sideToMove][ROOK] | board->pieces[board->sideToMove][QUEEN])
        & getRookAttacks(board->occ[BOTH], sq);
    attackers |= board->pieces[board->sideToMove][KING] & kingTable[sq];

    return attackers;
}

void generateLegalMoves(MoveList* moveList, Board* board) {

    enumeratePawnMoves(moveList, board);
    enumerateKnightMoves(moveList, board);
    enumerateBishopMoves(moveList, board);
    enumerateRookMoves(moveList, board);
    enumerateQueenMoves(moveList, board);
    enumerateKingMoves(moveList, board);
    
    for (int moveInd = 0; moveInd < moveList->end; moveInd++) {

        History h;
        

        makeMove(board, &h, moveList->moveArray[moveInd]);

        if (getSquareAttackers(board, getLSB(board->pieces[!board->sideToMove][KING]))) {
            moveList->moveArray[moveInd] = moveList->moveArray[moveList->end - 1];
            --moveList->end;
            --moveInd;
        }

        else if (getMoveType(moveList->moveArray[moveInd]) == CASTLING) {

            Bitboard castleChecks = 0ULL;
            Square target = getTargetSq(moveList->moveArray[moveInd]);

            if (board->sideToMove) {

                if (target == G1) {
                    castleChecks = getSquareAttackers(board, E1) | getSquareAttackers(board, F1);
                }

                else if (target == C1) {
                    castleChecks = getSquareAttackers(board, E1) | getSquareAttackers(board, D1);
                }
            }
            
            else {
                
                if (target == G8) {
                    castleChecks = getSquareAttackers(board, E8) | getSquareAttackers(board, F8);
                }
                else if (target == C8) {
                    castleChecks = getSquareAttackers(board, E8) | getSquareAttackers(board, D8);
                }
            }

            if (castleChecks) {
                moveList->moveArray[moveInd] = moveList->moveArray[moveList->end - 1];
                --moveList->end;
                --moveInd;
            }
        }

        unmakeMove(board, &h);
    }
}