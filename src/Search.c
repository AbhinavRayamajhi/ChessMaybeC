#include "../include/Search.h"

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

int negamax(Board* board, int depth) {

    if (depth == 0) {
        return evaluate(board);
    }

    MoveList moveList;
    moveList.end = 0;

    int best = INT32_MIN + 1;
    generateLegalMoves(&moveList, board);

    for (int i = 0; i < moveList.end; ++i) {
        History history;
        makeMove(board, &history, moveList.moveArray[i]);
        int score = -negamax(board, depth - 1);
        unmakeMove(board, &history);

        if (score > best) {
            best = score;
        }
    }
    return best;
}
    
Move findBestMove(Board* board, int depth) {

    MoveList moveList;
    moveList.end = 0;

    int best = INT32_MIN + 1;
    Move bestMove = 0;
    
    generateLegalMoves(&moveList, board);

    for (int i = 0; i < moveList.end; ++i) {
        History history;
        makeMove(board, &history, moveList.moveArray[i]);
        int score = -negamax(board, depth - 1);
        unmakeMove(board, &history);

        if (score > best) {
            best = score;
            bestMove = moveList.moveArray[i];
        }
    }

    return bestMove;
}