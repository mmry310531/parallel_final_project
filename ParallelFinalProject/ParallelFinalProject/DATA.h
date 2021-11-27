#pragma once
#include "Def.h"
#define PLAYER_SIDE 0
#define COMPUTER_SIDE 1



#define BLACK 1
#define WHITE 0

#define KING 1
#define QUEEN 2
#define BISHOP 3
#define KNIGHT 4
#define ROOK 5
#define PAWN 6



#define BColor 0
#define BPiece 1
#define NONE -1

#define A8 0
#define A7 8
#define A6 16
#define A5 24
#define A4 32
#define A3 40
#define A2 48
#define A1 56

#define B8 1
#define B7 9
#define B6 17
#define B5 25
#define B4 33
#define B3 41
#define B2 49
#define B1 57

#define C8 2
#define C7 10
#define C6 18
#define C5 26
#define C4 34
#define C3 42
#define C2 50
#define C1 58

#define D8 3
#define D7 11
#define D6 19
#define D5 27
#define D4 35
#define D3 43
#define D2 51
#define D1 59

#define E8 4
#define E7 12
#define E6 20
#define E5 28
#define E4 36
#define E3 44
#define E2 52
#define E1 60

#define F8 5
#define F7 13
#define F6 21
#define F5 29
#define F4 37
#define F3 45
#define F2 53
#define F1 61

#define G8 6
#define G7 14
#define G6 22
#define G5 30
#define G4 38
#define G3 46
#define G2 54
#define G1 62

#define H8 7
#define H7 15
#define H6 23
#define H5 31
#define H4 39
#define H3 47
#define H2 55
#define H1 63

#define ROW(x)			(x >> 3)
#define COL(x)			(x & 7)

// static
extern int init_color[64];
extern int init_piece[64];

extern int move_offset[8];
extern int knight_jump[8];
// dynamic
extern int board[2][64];
extern Hist_t history[1024];
extern MoveByte_set gen_dat[4096];
extern int first_move[32];
extern MoveByte pv[32][32];
extern int pv_length[32];
extern int bestEval;
extern int side;
extern int xside;
extern int ply;
extern int hply;
extern char castle;

extern bool AUTO;

extern int node;

extern int NumSquaresToEdge[64][8];
extern int NumSquareToKnight[8];
extern int numSquareToKnight_index;

extern const int pawn_score[64];
extern const int knight_score[64];
extern const int bishop_score[64];
extern const int rook_score[64];
extern const int flip[64];