#include "stdio.h"
#include "stdlib.h"
#include "def.h"

#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1pp3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b - e3 e6 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"

int main() {
    AllInit();

    S_BOARD board[1];

    ParseFen(ROOKS, board);

    PrintBoard(board)
;

    S_MOVELIST list[1];
    GenerateAllMoves(board, list);




    return 0;
}