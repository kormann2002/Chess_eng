#include "stdio.h"
#include "def.h"

int Checkboard(const S_BOARD *pos){
    int t_pceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int t_bigPce[2] = {0, 0};
    int t_majPce[2] = {0, 0};
    int t_minPce[2] = {0, 0};
    int t_material[2] = {0, 0};

    int sq64, t_piece, t_pce_num, sq120, colour, pcount;

    U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

    t_pawns[WHITE] = pos->pawns[WHITE];
    t_pawns[BLACK] = pos->pawns[BLACK];
    t_pawns[BOTH] = pos->pawns[BOTH];

    for(t_piece = wP; t_piece <= bK; ++t_piece){ //check piece list
        for(t_pce_num = 0; t_pce_num < pos->pceNum[t_piece]; ++t_pce_num){
            sq120 = pos->pList[t_piece][t_pce_num];
            ASSERT(pos->pieces[sq120] == t_piece);
        }
    }

    for(sq64 = 0; sq64 <64; ++sq64){ //check piece count and other counters
        sq120 = SQ120(sq64);
        t_piece = pos->pieces[sq120];
        t_pceNum[t_piece]++;
        colour = PieceCol[t_piece];
        if(PieceBig[t_piece] == TRUE) t_bigPce[colour]++;
        if(PieceMin[t_piece] == TRUE) t_minPce[colour]++;
        if(PieceMaj[t_piece] == TRUE) t_majPce[colour]++;

        t_material[colour] += PieceVal[t_piece];
    }

    for(t_piece = wP; t_piece<= bK; ++t_piece){
        ASSERT(t_pce_num[t_piece] == pos->pceNum[t_piece]);
    }
    
    pcount = CNT(t_pawns[WHITE]);
    ASSERT(pcount == pos->pceNum[wP]);
    pcount = CNT(t_pawns[BLACK]);
    ASSERT(pcount == pos->pceNum[bP]);
    pcount = CNT(t_pawns[BOTH]);
    ASSERT(pcount == pos->pceNum[bP] + pos->pceNum[wP]);

    while (t_pawns[WHITE])
    {
        sq64 = POP(&t_pawns[WHITE]);
        ASSERT(pos->pieces[SQ120(sq64)] == wP)
    }
    while (t_pawns[BLACK])
    {
        sq64 = POP(&t_pawns[BLACK]);
        ASSERT(pos->pieces[SQ120(sq64)] == bP)
    }
    while (t_pawns[BOTH])
    {
        sq64 = POP(&t_pawns[BOTH]);
        ASSERT(pos->pieces[SQ120(sq64)] == wP || pos->pieces[SQ120(sq64)] == bP)
    }

    ASSERT(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
    ASSERT(t_minPce[WHITE]==pos->t_minPce[WHITE] && t_minPce[BLACK]==pos->t_minPce[BLACK]);
    ASSERT(t_majPce[WHITE]==pos->t_majPce[WHITE] && t_majPce[BLACK]==pos->t_majPce[BLACK]);
    ASSERT(t_bigPce[WHITE]==pos->t_bigPce[WHITE] && t_bigPce[BLACK]==pos->t_bigPce[BLACK]);

    ASSERT(pos->side==WHITE || pos->side==BLACK);
    ASSERT(GeneratePosKey(pos)==pos->posKey);

    ASSERT(pos->enPas==NO_SQ || (RanksBrd[pos->enPas]==RANK_6 && pos->side == WHITE) || (RanksBrd[pos->enPas]==RANK_3 && pos->side == BLACK));
    
    ASSERT(pos->pieces[pos->kingSq[WHITE]] == wK);
    ASSERT(pos->pieces[pos->kingSq[BLACK]] == bK);

    return TRUE;

}

void UpdateListMaterial(S_BOARD *pos){ //updates S_BOARD arrays
    int piece, sq, colour, i;

    for(i = 0; i < BRD_SQ_NUM; ++i){ //loop through 120 board
        sq = i;
        piece = pos->pieces[i];
        if(piece!=OFFBOARD && piece !=EMPTY){ //if inside playable board
            colour = PieceCol[piece];//get piece colour

            //increment piece type counters
            if(PieceBig[piece]==TRUE) pos->bigPce[colour]++;
            if(PieceMin[piece]==TRUE) pos->minPce[colour]++;
            if(PieceMaj[piece]==TRUE) pos->majPce[colour]++;

            pos->material[colour] += PieceVal[piece]; //add value to material

            pos->pList[piece][pos->pceNum[piece]] = sq; //add the square to the piece
            pos->pceNum[piece]++; //increment the piece counter

            if(piece==wK || piece==bK) pos->kingSq[colour] = sq; //king position
            
            if(piece==wP || piece ==bP){ //set the bits on the bitboard for the pawns
                SETBIT(pos->pawns[colour], SQ64(sq));
                SETBIT(pos->pawns[BOTH], SQ64(sq));
            }
        }
    }
}


int ParseFen(char *fen, S_BOARD *pos){
    ASSERT(fen!NULL);
    ASSERT(pos!=NULL);

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0; //store the number of files to set to empty
    int i = 0;
    int sq64 = 0;
    int sq120 = 0;

    ResetBoard(pos);

    while ((rank >= RANK_1) && *fen){
        count = 1;
        switch (*fen){
            //if a piece, store it
            case 'p' : piece = bP ; break;
            case 'r' : piece =  bR; break;
            case 'n' : piece =  bN; break;
            case 'b' : piece = bB ; break;
            case 'k' : piece =  bK; break;
            case 'q' : piece = bQ; break;
            case 'P' : piece = wP; break;
            case 'R' : piece =  wR; break;
            case 'N' : piece = wN; break;
            case 'B' : piece = wB; break;
            case 'K' : piece = wK; break;
            case 'Q' : piece = wQ; break;

            //if a number
            case '1' :
            case '2' : 
            case '3' : 
            case '4' : 
            case '5' : 
            case '6' : 
            case '7' :
            case '8' : 
                piece = EMPTY; //no piece on the square
                count = *fen - '0'; //subtract the ascii value of 0 to get the number
                break;

            
            case '/': //means it's a new rank
            case ' ': //means we're at the end
                rank--; //decriment the rank
                file = FILE_A; //going back to file_a
                fen++; //next char on FEN
                continue;

            default: printf("FEN error |n"); return -1;
        };

        for(i = 0; i < count; i++){
            sq64 = rank * 8 + file; //get the square we're in
            sq120 = SQ120(sq64); //translate to 120 board
            if(piece!=EMPTY){ 
                pos->pieces[sq120] = piece; //set the square to the correct piece in case it's not empty
            }
            file++; //next file
        }
        fen++; //next char on fen
    }
    ASSERT(*fen == 'w' || *fen == 'b' );

    pos->side = (*fen == 'w') ? WHITE : BLACK;//if points to 'w' is white else black
    fen+=2;

    for(i = 0; i < 4; i++){ //loop through castle permissions
        if(*fen == ' ') break;
        switch (*fen)
        { //set the correspondent bit for the castle
        case 'K' : pos->castlePerm |= WKCA; break;
        case 'Q' : pos->castlePerm |= WQCA; break;
        case 'k' : pos->castlePerm |= BKCA; break;
        case 'q' : pos->castlePerm |= BQCA; break;
        default: break;
        }
        fen++;
    }fen++; //move to enpPassant square
    
    ASSERT(pos->castlePerm>=0 && pos->castlePerm<=15);

    if(*fen != '-'){
        file = fen[0] - 'a'; 
        rank = fen[1] - '1';

        ASSERT(file>=FILE_A && file <= FILE_H);
        ASSERT(rank>= RANK_1 && rank <= RANK_8);

        pos->enPas = FR2SQ(file, rank); //storing the enPassant square
    }
    pos->posKey = GeneratePosKey(pos);
    UpdateListMaterial(pos);
    return 0;
}


void ResetBoard(S_BOARD *pos){ //function to reset the board

    int i = 0;

    for(i = 0; i < BRD_SQ_NUM; ++i){
        pos->pieces[i] = OFFBOARD   ;
    }
    for(i = 0; i<64; ++i){
        pos->pieces[SQ120(i)] = EMPTY;
    }

    for(i = 0;i < 3; ++i){
        pos->bigPce[i] = 0;
        pos->majPce[i] = 0;
        pos->minPce[i] = 0;
        pos->pawns[i] = 0ULL;
    }

    for(i = 0; i < 13; ++i){
        pos->pceNum[i] = 0;
    }

    pos->kingSq[WHITE] = pos->kingSq[BLACK] = NO_SQ;

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;

    pos->ply = 0;
    pos->hisPly; 0; 

    pos->castlePerm = 0;

    pos->posKey = 0ULL;

}

void PrintBoard(const S_BOARD *pos){

    int sq,file,rank,piece;

    printf("\nGame Board: \n\n");

    for(rank = RANK_8; rank>= RANK_1; rank--){
        printf("%d  ",rank+1);
        for(file = FILE_A; file<= FILE_H; file++){
            sq = FR2SQ(file,rank);
            piece = pos->pieces[sq];
            printf("%3c", PceChar[piece]);
        }
        printf("\n");
    }
    printf("\n  ");
    for(file = FILE_A; file<= FILE_H; file++){
        printf("%3c",'a'+file);
    }
    printf("\n");
    printf("side:%c\n",SideChar[pos->side]);
    printf("enPas:%d\n",pos->enPas);
    printf("castle:%c%c%c%c\n", //see if correspondent bit is set using a AND operation
    pos->castlePerm & WKCA ? 'K' : '-',
    pos->castlePerm & WQCA ? 'Q' : '-',
    pos->castlePerm & BKCA ? 'k' : '-',
    pos->castlePerm & BQCA ? 'q' : '-'
    );
    printf("PosKey:%11X\n",pos->posKey);
}