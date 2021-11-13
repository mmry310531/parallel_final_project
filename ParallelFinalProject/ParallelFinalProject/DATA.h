#pragma once

#define PLAYER_SIDE 0
#define COMPUTER_SIDE 1
#define AUTO 2


#define BLACK 1
#define WHITE 2

#define KING 1
#define QUEEN 2
#define BISHOP 3
#define KNIGHT 4
#define CASTLE 5

#define BColor 0
#define BPiece 1

int board[2][64];


int init_color[64] = {
	 1,  1,  1,  1,  1,  1,  1,  1,
	 1,  1,  1,  1,  1,  1,  1,  1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	 2,  2,  2,  2,  2,  2,  2,  2,
	 2,  2,  2,  2,  2,  2,  2,  2,
};

int init_piece[64] = {
	5, 4, 3, 2, 1, 3, 4, 5, 
	1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 4, 3, 2, 1, 3, 4, 5,
}