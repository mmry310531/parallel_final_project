#pragma once
#pragma warning 4996



typedef struct MoveByte{
	int from;
	int to;
	int promote;
	int castle;
	bool pawn;
	bool pawn2;
	bool capture;
	bool en_capture;
	// 0 is can castle ; 1 is left ; 2 is right ; -1 can't castle
	bool legal;
} MoveByte;

typedef struct MoveByte_set {
	MoveByte movebyte;
	int score;
} MoveByte_set;

typedef struct Hist{
	char castle;
	int board[2][64];
	int ep;
} Hist_t;

