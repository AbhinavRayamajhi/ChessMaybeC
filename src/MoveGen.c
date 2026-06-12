#include "../include/MoveGen.h"

Bitboard getRookAttacks(Bitboard occ, int sq) {

    Bitboard rookMask = rookTable[sq];
    uint64_t index = ((rookMask & occ) * ROOK_MAGICS[sq]) >> (64 - popCount(rookMask));
    return rookAttackTable[sq][index];
}

Bitboard getBishopAttacks(Bitboard occ, int sq) {

    Bitboard bishopMask = bishopTable[sq];
    uint64_t index = ((bishopMask & occ) * BISHOP_MAGICS[sq]) >> (64 - popCount(bishopMask));
    return bishopAttackTable[sq][index];
}

void enumerateKnightMoves(MoveList* moveList, Board* board) {

    Bitboard knights = board->sideToMove ? board->pieces[BLACK][KNIGHT] : board->pieces[WHITE][KNIGHT];
    Bitboard friends = board->sideToMove ? board->occ[BLACK] : board->occ[WHITE];

    while(knights) {

        Square start = getLSB(knights);
        clearLSB(knights);

        Bitboard attacks = knightTable[start] & ~friends;
        
        while(attacks) {

            Square target = getLSB(attacks);
            clearLSB(attacks);

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }
}

void enumerateKingMoves(MoveList* moveList, Board* board) {
    Bitboard king = board->sideToMove ? board->pieces[BLACK][KING] : board->pieces[WHITE][KING];
    Bitboard friends = board->sideToMove ? board->occ[BLACK] : board->occ[WHITE];

    Square start = getLSB(king);
    Bitboard attacks = kingTable[start] & ~friends;

    while (attacks) {

        Square target = getLSB(attacks);
        clearLSB(attacks);

        addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
    }

    // TODO: Square check required to add castling moves

    if (!board->sideToMove && (board->castlingRight & WHITE_CASTLING)) {

        if((board->castlingRight & WHITE_KINGSIDE) && !(board->occ[BOTH] & WHITE_OO)) {

            addMove(moveList, make(E1, G1, NO_PROMOTION_PIECE, CASTLING));
        }
        if((board->castlingRight & WHITE_QUEENSIDE) && !(board->occ[BOTH] & WHITE_OOO)) {

            addMove(moveList, make(E1, C1, NO_PROMOTION_PIECE, CASTLING));
        }
    }
    else if(board->sideToMove && (board->castlingRight & BLACK_CASTLING)) {

        if((board->castlingRight & BLACK_KINGSIDE) && !(board->occ[BOTH] & BLACK_OO)) {

            addMove(moveList, make(E8, G8, NO_PROMOTION_PIECE, CASTLING));
        }
        if((board->castlingRight & BLACK_QUEENSIDE) && !(board->occ[BOTH] & BLACK_OOO)) {

            addMove(moveList, make(E8, C8, NO_PROMOTION_PIECE, CASTLING));
        }
    }
}

void enumerateRookMoves(MoveList* moveList, Board* board) {

    Bitboard rooks = board->sideToMove ? board->pieces[BLACK][ROOK] : board->pieces[WHITE][ROOK];
    Bitboard friends = board->sideToMove ? board->occ[BLACK] : board->occ[WHITE];

    while(rooks) {

        Square start = getLSB(rooks);
        clearLSB(rooks);

        Bitboard attacks = getRookAttacks(board->occ[BOTH], start) & ~friends;
        
        while(attacks) {

            Square target = getLSB(attacks);
            clearLSB(attacks);

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }
}

void enumerateBishopMoves(MoveList* moveList, Board* board) {
    
    Bitboard bishops = board->sideToMove ? board->pieces[BLACK][BISHOP] : board->pieces[WHITE][BISHOP];
    Bitboard friends = board->sideToMove ? board->occ[BLACK] : board->occ[WHITE];

    while(bishops) {

        Square start = getLSB(bishops);
        clearLSB(bishops);

        Bitboard attacks = getBishopAttacks(board->occ[BOTH], start) & ~friends;
        
        while(attacks) {

            Square target = getLSB(attacks);
            clearLSB(attacks);

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }
}

void enumerateQueenMoves(MoveList* moveList, Board* board) {
    
    Bitboard queens = board->sideToMove ? board->pieces[BLACK][QUEEN] : board->pieces[WHITE][QUEEN];
    Bitboard friends = board->sideToMove ? board->occ[BLACK] : board->occ[WHITE];

    while(queens) {

        Square start = getLSB(queens);
        clearLSB(queens);

        Bitboard attacks = (getRookAttacks(board->occ[BOTH], start) | getBishopAttacks(board->occ[BOTH], start)) & ~friends;
        
        while(attacks) {

            Square target = getLSB(attacks);
            clearLSB(attacks);

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }
}

void enumeratePawnMoves(MoveList* moveList, Board* board) {

    Bitboard pawns = board->sideToMove ? board->pieces[BLACK][PAWN] : board->pieces[WHITE][PAWN];
    Bitboard enemies = board->sideToMove ? board->occ[WHITE] : board->occ[BLACK];

    Bitboard singlePush = singlePawnPush(pawns, board->sideToMove, board->occ[BOTH]);

    while (singlePush) {

        Square target = getLSB(singlePush);
        clearLSB(singlePush);
        Square start = target - (board->sideToMove ? -8 : 8);

        // promotion checks
        if (!board->sideToMove && target >= 56) {

            addMove(moveList, make(start, target, QUEEN, PROMOTION));
            addMove(moveList, make(start, target, ROOK, PROMOTION));
            addMove(moveList, make(start, target, BISHOP, PROMOTION));
            addMove(moveList, make(start, target, KNIGHT, PROMOTION));
        }
        else if (board->sideToMove && target <= 7) {

            addMove(moveList, make(start, target, QUEEN, PROMOTION));
            addMove(moveList, make(start, target, ROOK, PROMOTION));
            addMove(moveList, make(start, target, BISHOP, PROMOTION));
            addMove(moveList, make(start, target, KNIGHT, PROMOTION));
        }
        else {

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }

    Bitboard doublePush = doublePawnPush(pawns, board->sideToMove, board->occ[BOTH]);

    while (doublePush) {

        Square target = getLSB(doublePush);
        clearLSB(doublePush);
        Square start = target - (board->sideToMove ? -16 : 16);

        addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
    }

    Bitboard leftAttacks = pawnLeftAttack(pawns, board->sideToMove) & enemies;

    while (leftAttacks) {

        Square target = getLSB(leftAttacks);
        clearLSB(leftAttacks);
        Square start = target - (board->sideToMove ? -9 : 7);

         // promotion checks
        if (!board->sideToMove && target >= 56) {

            addMove(moveList, make(start, target, QUEEN, PROMOTION));
            addMove(moveList, make(start, target, ROOK, PROMOTION));
            addMove(moveList, make(start, target, BISHOP, PROMOTION));
            addMove(moveList, make(start, target, KNIGHT, PROMOTION));
        }
        else if (board->sideToMove && target <= 7) {

            addMove(moveList, make(start, target, QUEEN, PROMOTION));
            addMove(moveList, make(start, target, ROOK, PROMOTION));
            addMove(moveList, make(start, target, BISHOP, PROMOTION));
            addMove(moveList, make(start, target, KNIGHT, PROMOTION));
        }
        else {

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }

    Bitboard rightAttacks = pawnRightAttack(pawns, board->sideToMove) & enemies;

    while (rightAttacks) {

        Square target = getLSB(rightAttacks);
        clearLSB(rightAttacks);
        Square start = target - (board->sideToMove ? -7 : 9);

         // promotion checks
        if (!board->sideToMove && target >= 56) {

            addMove(moveList, make(start, target, QUEEN, PROMOTION));
            addMove(moveList, make(start, target, ROOK, PROMOTION));
            addMove(moveList, make(start, target, BISHOP, PROMOTION));
            addMove(moveList, make(start, target, KNIGHT, PROMOTION));
        }
        else if (board->sideToMove && target <= 7) {

            addMove(moveList, make(start, target, QUEEN, PROMOTION));
            addMove(moveList, make(start, target, ROOK, PROMOTION));
            addMove(moveList, make(start, target, BISHOP, PROMOTION));
            addMove(moveList, make(start, target, KNIGHT, PROMOTION));
        }
        else {

            addMove(moveList, make(start, target, NO_PROMOTION_PIECE, NORMAL));
        }
    }

    if (board->enPassantSq != NONE) {

        Bitboard enPassant = 0ULL;
        setSq(enPassant, board->enPassantSq);

        if (pawnLeftAttack(pawns, board->sideToMove) & enPassant) {

            Square start = board->enPassantSq + (board->sideToMove ? 9 : -7);
            addMove(moveList, make(start, board->enPassantSq, NO_PROMOTION_PIECE, EN_PASSANT));
        }
        if (pawnRightAttack(pawns, board->sideToMove) & enPassant) {

            Square start = board->enPassantSq + (board->sideToMove ? 7 : -9);
            addMove(moveList, make(start, board->enPassantSq, NO_PROMOTION_PIECE, EN_PASSANT));
        }
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