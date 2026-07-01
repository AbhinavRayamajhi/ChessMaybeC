#include "MaskGen.h"
#include "Test.h"
#include "Magic.h"
#include "Board.h"
#include "Search.h"

int main() {

    initMasks();
    initMagicTables();

    runAllPerftTests();

    // Board board = getBoardFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    // int nodes = 0;
    // printMove(findBestMove(&board, 5, &nodes));
    // printf("\n%d\n", nodes);
    // return 0;
}