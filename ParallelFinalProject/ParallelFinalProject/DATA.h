#ifndef DATA_H
#define DATA_H

#define PLAYER_SIDE 0
#define COMPUTER_SIDE 1



#define BLACK 1
#define WHITE 0

#define KING 1
#define QUEEN 2
#define BISHOP 3
#define KNIGHT 4
#define CASTLE 5
#define PAWN 6

#define BColor 0
#define BPiece 1




// static
extern int init_color[64];
extern int init_piece[64];


// dynamic
extern int board[2][64];
extern int side;
extern int ply;
extern int half_ply;

extern bool AUTO;

#endif \* DATA_H*\

