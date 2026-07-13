#include "Search.h"

#include "Position.h"

const int materialVal[] = { 100, 320, 330, 500, 900 };

int evaluate(const Board* board) {

    int whiteMaterial = 0, blackMaterial = 0;

    for (Piece p = PAWN; p < PIECE_COUNT - 1; ++p) {

        whiteMaterial += popCount(board->pieces[WHITE][p]) * materialVal[p];
        blackMaterial += popCount(board->pieces[BLACK][p]) * materialVal[p];
    }

    int score = whiteMaterial - blackMaterial;
    return board->sideToMove == WHITE ? score : -score;
}

int negamax(Board* board, Depth depth, int* nodes, Score alpha, Score beta) {

    if (depth == 0) {
        return evaluate(board);
    }

    MoveList moveList = { .end = 0 };
    generateLegalMoves(&moveList, board);

    for (int i = 0; i < moveList.end; ++i) {

        History history;
        makeMove(board, &history, moveList.moveArray[i]);
        Score score = -negamax(board, depth - 1, nodes, -beta, -alpha);
        unmakeMove(board, &history);

        (*nodes)++;

        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
    
Move findBestMove(Board* board, Depth depth, int* nodes) {

    Score best = MIN_SCORE;
    Move bestMove = 0;

    MoveList moveList = { .end = 0 };
    generateLegalMoves(&moveList, board);

    for (int i = 0; i < moveList.end; ++i) {

        History history;
        makeMove(board, &history, moveList.moveArray[i]);
        Score score = -negamax(board, depth - 1, nodes, MIN_SCORE, MAX_SCORE);
        unmakeMove(board, &history);

        (*nodes)++;
        if (score > best) {
            best = score;
            bestMove = moveList.moveArray[i];
        }
    }

    return bestMove;
}