#include "stdio.h"
#include "stdlib.h"
#include "def.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"


int main() {
    AllInit();

    int move = 0;
    int from = A2;
    int to = H7;
    int cap = wR;
    int prom = bB;

    move = ((from) | (to<<7) | (cap<<14) | (prom<<20));

    printf("from: %d to: %d cap: %d prom: %d\n",
    FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move)
    );

    printf("Algebraic from: %s\n", PrSq(from));
    printf("Algebraic to: %s\n", PrSq(to));
    printf("Algebraic move: %s\n", PrMove(move));

    



    
    return 0;
}