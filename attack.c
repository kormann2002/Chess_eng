#include "def.h"
#include <stdio.h>

//the square index where x piece would be attacking the square, 120 based 
const int KnDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int RkDir[4] = {-1, -10, 1, 10};
const int BiDir[4] = {-1, -11, 11, 9};
const int KiDir[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

int SqAttacked(const int sq, const int side, const S_BOARD *pos){

    int pce, i, t_sq, dir;

    ASSERT(SqOnboard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(pos));

    //pawns
    if(side == WHITE){
        if(pos->pieces[sq-11] == wP || pos->pieces[sq-9] ==wP){
            return TRUE;
        }
    }else{
        if(pos->pieces[sq+11] == bP || pos->pieces[sq+9] ==bP){
            return TRUE;
        }
    }
    //knights
    for(i = 0; i < 8; ++i){
        pce = pos->pieces[sq + KnDir[i]];
        if(IsKn(pce) && PieceCol[pce] == side){
            return TRUE;
        }
    }
    //rooks and queens
    for(i = 0; i < 4; ++i){
        dir = RkDir[i];
        pce = pos->pieces[t_sq];
        while(pce != OFFBOARD){
            if(pce != EMPTY){
                if(IsRQ(pce) && PieceCol[pce] == side){
                    return TRUE;
                }break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }
    //bishops and queens
    for(i = 0; i < 4; ++i){
        dir = BiDir[i];
        pce = pos->pieces[t_sq];
        while(pce != OFFBOARD){
            if(pce != EMPTY){
                if(IsBQ(pce) && PieceCol[pce] == side){
                    return TRUE;
                }break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }
    //king
    for(i = 0; i < 8; ++i){
        pce = pos->pieces[sq + KiDir[i]];
        if(IsKi(pce) && PieceCol[pce] == side){
            return TRUE;
        }
    }

    return FALSE;
    
}
