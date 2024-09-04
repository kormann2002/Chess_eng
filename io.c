#include "def.h"
#include <stdio.h>
#include <string.h>


char *PrSq(const int sq){ 
    static char SqStr[3];

    int file = FilesBrd[sq];
    int rank = RanksBrd[sq];

    sprintf(SqStr, "%c%c", ('a'+file), ('1'+rank)); //put the file and rank into the string
    return SqStr;
}

char *PrMove(const int move){
    static char MvStr[6+1];
    memset(MvStr, 0, 7);

    //file or rank, from or to
    int ff = FilesBrd[FROMSQ(move)];
    int rf = RanksBrd[FROMSQ(move)];
    int ft = FilesBrd[TOSQ(move)];
    int rt = RanksBrd[TOSQ(move)];

    int promoted = PROMOTED(move);

    if(promoted) {
        char pchar = 'q';
        if(IsKn(promoted)){
            pchar = 'n';
        } else if(IsRQ(promoted) && !IsBQ(promoted)){
            pchar = 'r';
        } else if(!IsRQ(promoted) && IsBQ(promoted)){
            pchar = 'b';
        }
        sprintf(MvStr, "%c%c%c%c%c" , ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
    } 
    else{
        sprintf(MvStr, "%c%c%c%c" , ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
    }

    return MvStr;
}

void PrintMoveList(const S_MOVELIST *list){

    int i = 0;
    int score = 0;
    int move = 0;
    printf("MoveList:%d\n", list->count);

    for(i = 0; i < list->count; ++i){

        move = list->moves[i].move;
        score = list->moves[i].score;

        printf("Move:%d > %s (score: %d)\n", i+1, PrMove(move), score);
    }
    printf("MoveList Total %d Moves\n\n", list->count);

}