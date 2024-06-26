//init.c

#include "def.h"
#include "stdlib.h"

//macro for filling all 64 bits of the variable with random numbers
#define RAND_64 (   (U64) rand() | \
                (U64) rand() << 15 | \
                (U64) rand() << 30 | \
                (U64) rand() << 45 | \
                ((U64) rand() & 0xf) << 60 )

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

//bitmasks
U64 SetMask[64];
U64 ClearMask[64];

//hashkeys
U64 PieceKeys[13][120]; //[pieces][squares]
U64 SideKey; //key to hash in when white moves
U64 CastleKeys[16];

int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

void InitFIlesRanksBrd(){

    int i = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;

    for(i = 0; i < BRD_SQ_NUM; ++i){
        FilesBrd[i] = OFFBOARD;
        RanksBrd[i] = OFFBOARD;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank){
        for(file = FILE_A; file <= FILE_H; ++file){
            sq = FR2SQ(file, rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }
}

void InitHashKeys(){
    int i = 0;
    int j = 0;

    for(i = 0; i < 13; ++i){
        for(j = 0; j < 120; ++j){
            PieceKeys[i][j] = RAND_64;
        }
    }
    SideKey = RAND_64;
    for(i = 0; i < 16; ++i){
        CastleKeys[i] = RAND_64;
    }
}

void InitBitMask(){
    int i = 0;

    for(i = 0; i < 64; i++){
        SetMask[i] = 0ULL;
        ClearMask[i] = 0ULL;
    }
    for(i = 0; i < 64; i++){
        SetMask[i] |= (1ULL << i); //shift a 1 to the same place as the index
        ClearMask[i] = ~SetMask[i]; //not SetMask
    }
}


void InitSq120To64(){

    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    for(index = 0; index < BRD_SQ_NUM; ++index){
        Sq120ToSq64[index] = 65;
    }

    for(index = 0; index < 64; ++index){
        Sq64ToSq120[index] = 125;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank){
        for(file = FILE_A; file <= FILE_H; ++file){
            sq = FR2SQ(file,rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

void AllInit(){
    InitSq120To64();
    InitBitMask();
    InitHashKeys();
    InitFIlesRanksBrd();
}