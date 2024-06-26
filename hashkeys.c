#include "stdio.h"
#include "def.h"

U64 GeneratePosKey(const S_BOARD *pos){
    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    //pieces
    for(sq = 0; sq < BRD_SQ_NUM; ++sq) {//loop through all sqrs on board
        piece = pos ->pieces[sq]; //get the piece at the square position
        if(piece!= NO_SQ && piece!=EMPTY && piece != OFFBOARD){ //procede when there's a piece at the square
            ASSERT(piece>=wP && piece<=bK); //must be a piece
            finalKey ^= PieceKeys[piece][sq]; //hash the piece into the finalKey
        }
    }

    if(pos->side == WHITE){ //hash the sideKey when white moves
        finalKey ^=SideKey;
    }

    if(pos->enPas != NO_SQ){ //check for an enPassant square
        ASSERT(pos->enPas>=0 && pos->enPas<BRD_SQ_NUM); //check if enPas square is valid
        finalKey ^= PieceKeys[EMPTY][pos->enPas]; //hash it
    }

    ASSERT(pos->castlePerm>=0 && pos->castlePerm<=15); //check if castlePerm is valid

    finalKey ^= CastleKeys[pos->castlePerm]; //Hash it 

    return finalKey;
}