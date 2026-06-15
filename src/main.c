#include "../include/MaskGen.h"
#include "../include/Test.h"
#include "../include/Magic.h"
#include "../include/Board.h"
#include "../include/Search.h"

int main() {

    initMasks();
    initMagicTables();

    Board board = getInitialBoard();
    printMove(findBestMove(&board, 3));
    
    return 0;
}