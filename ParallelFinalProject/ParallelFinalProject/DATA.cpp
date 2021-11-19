#include "DATA.h"
#include "Def.h"


int board[2][64];
Hist_t history[1024];

MoveByte_set gen_dat[4096];

int first_move[32];
int side;
int xside;
int ply;
int hply;
char castle;
bool AUTO;

int move_offset[8] = { -8, 8, -1, 1, -9, 9, -7, 7 };
int knight_jump[8] = { 15, 17, -17, -15, 10, -6, 6, -10 };
int init_color[64] = {
	 1,  1,  1,  1,  1,  1,  1,  1,
	 1,  1,  1,  1,  1,  1,  1,  1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	 0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0
};
int init_piece[64] = {
	 5,  4,  3,  2,  1,  3,  4,  5,
	 6,  6,  6,  6,  6,  6,  6,  6,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	 6,  6,  6,  6,  6,  6,  6,  6,
	 5,  4,  3,  2,  1,  3,  4,  5 

};

int NumSquaresToEdge[64][8] = { 0 };
int NumSquareToKnight[8] = { 0 };
int numSquareToKnight_index = 0;