﻿#include "DATA.h"
#include "Def.h"


__declspec(thread) int board[2][64];
__declspec(thread) Hist_t history[1024];
__declspec(thread) MoveByte_set MoveSet[4096];
__declspec(thread) int branchNodes[32];
__declspec(thread) MoveByte pv[32][32];
__declspec(thread) int pv_length[32];
__declspec(thread) int side;
__declspec(thread) int xside;
__declspec(thread) int ply;
__declspec(thread) int hply;
__declspec(thread) int ep;
__declspec(thread) char castle;

MoveByte best_pv[32];
int best_pv_length;
int thread_num;

MoveByte bestMove;
int bestEval;

bool AUTO;

int node;
int totalNodes;
int move_offset[8] = { -8, 8, -1, 1, -9, 9, -7, 7 };
int knight_jump[8] = { 15, 17, -17, -15, 10, -6, 6, -10 };

//int init_color_end[64] = {
//	-1, -1, -1, -1,  1, -1, -1, -1,
//	 0, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1,  0, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//
//};
//int init_piece_end[64] = {
//	-1, -1, -1, -1,  1, -1, -1, -1,
//	 2, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1,  1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//	-1, -1, -1, -1, -1, -1, -1, -1,
//
//};


int init_color[64] = {
	 1,  1,  1,  1,  1,  1,  1,  1,
	 1,  1,  1,  1,  1,  1,  1,  1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	 0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,

};
int init_piece[64] = {
	 5,  4,  3,  2,  1,  3,  4,  5,
	 6,  6,  6,  6,  6,  6,  6,  6,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	 6,  6,  6,  6,  6,  6,  6,  6,
	 5,  4,  3,  2,  1,  3,  4,  5,

};

//int init_color[64] = {
//	BLACK,	NONE,	BLACK,	BLACK,	NONE,	BLACK,	BLACK,	BLACK,
//	BLACK,	BLACK,	BLACK,	NONE,	NONE,	BLACK,	BLACK,	BLACK,
//	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
//	NONE,	WHITE,	NONE,	NONE,	WHITE,	NONE,	NONE,	NONE,
//	NONE,	NONE,	NONE,	NONE,	WHITE,	NONE,	NONE,	NONE,
//	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
//	WHITE,	WHITE,	BLACK,	NONE,	WHITE,	WHITE,	WHITE,	WHITE,
//	WHITE,	WHITE,	WHITE,	NONE,	NONE,	NONE,	NONE,	WHITE
//
//};
//int init_piece[64] = {
//	ROOK,   NONE,   BISHOP,   KING,   NONE,   BISHOP,   KNIGHT,   ROOK,
//	PAWN,   PAWN,	PAWN,	NONE,   NONE,   PAWN,   PAWN,   PAWN,
//	NONE,   NONE,	NONE,   NONE,   NONE,   NONE,   NONE,   NONE,
//	NONE,   BISHOP,   NONE,   NONE,   KNIGHT,   NONE,	NONE,   NONE,
//	NONE,   NONE,   NONE,   NONE,   PAWN,	NONE,	NONE,	NONE,
//	NONE,	NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,
//	PAWN,	PAWN,   KNIGHT,   NONE,   KING,   PAWN,   PAWN,   PAWN,
//	ROOK,   KNIGHT,   BISHOP,	NONE,   NONE,   NONE,   NONE,	ROOK
//
//};

// pawn positional score
const int pawn_score[64] =
{
	90,  90,  90,  90,  90,  90,  90,  90,
	30,  30,  30,  40,  40,  30,  30,  30,
	20,  20,  20,  30,  30,  30,  20,  20,
	10,  10,  10,  20,  20,  10,  10,  10,
	 5,   5,  10,  20,  20,   5,   5,   5,
	 0,   0,   0,   5,   5,   0,   0,   0,
	 0,   0,   0, -10, -10,   0,   0,   0,
	 0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] =
{
	-10, -10, -10, -10, -10, -10, -10, -10,
	-10,   0,   0,   0,   0,   0,   0, -10,
	-10,   0,   5,   5,   5,   5,   0, -10,
	-10,   0,   5,  10,  10,   5,   0, -10,
	-10,   0,   5,  10,  10,   5,   0, -10,
	-10,   0,   5,   5,   5,   5,   0, -10,
	-10,   0,   0,   0,   0,   0,   0, -10,
	-10, -30, -10, -10, -10, -10, -30, -10
};


// bishop positional score
const int bishop_score[64] =
{
	-10, -10, -10, -10, -10, -10, -10, -10,
	-10,   0,   0,   0,   0,   0,   0, -10,
	-10,   0,   5,   5,   5,   5,   0, -10,
	-10,   0,   5,  10,  10,   5,   0, -10,
	-10,   0,   5,  10,  10,   5,   0, -10,
	-10,   0,   5,   5,   5,   5,   0, -10,
	-10,   0,   0,   0,   0,   0,   0, -10,
	-10, -10, -20, -10, -10, -20, -10, -10
};

// rook positional score
const int rook_score[64] =
{
	50,  50,  50,  50,  50,  50,  50,  50,
	50,  50,  50,  50,  50,  50,  50,  50,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,  10,  20,  20,  10,   0,   0,
	 0,   0,   0,  20,  20,   0,   0,   0

};

const int king_score[64] = {
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-40, -40, -40, -40, -40, -40, -40, -40,
	-20, -20, -20, -20, -20, -20, -20, -20,
	  0,  20,  40, -20,   0, -20,  40,  20
};

const int flip[64] = {
	 56,  57,  58,  59,  60,  61,  62,  63,
	 48,  49,  50,  51,  52,  53,  54,  55,
	 40,  41,  42,  43,  44,  45,  46,  47,
	 32,  33,  34,  35,  36,  37,  38,  39,
	 24,  25,  26,  27,  28,  29,  30,  31,
	 16,  17,  18,  19,  20,  21,  22,  23,
	  8,   9,  10,  11,  12,  13,  14,  15,
	  0,   1,   2,   3,   4,   5,   6,   7
};

int NumSquaresToEdge[64][8] = { 0 };
int NumSquareToKnight[8] = { 0 };
int numSquareToKnight_index = 0;