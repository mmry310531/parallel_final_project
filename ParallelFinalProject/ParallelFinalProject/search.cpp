#include <stdio.h>
#include <string.h>
#include "search.h"
#include "DATA.h"

int EvaluateBoard(int board_t[2][64]) {
	int score_WHITE = 0;
	int score_BLACK = 0;
	for (int i = 0; i < 64; i++) {
		if (board_t[BColor][i] == WHITE) {
			if (board_t[BPiece][i] == QUEEN) score_WHITE += 900;
			else if (board_t[BPiece][i] == BISHOP) score_WHITE += 300;
			else if (board_t[BPiece][i] == KNIGHT) score_WHITE += 300;
			else if (board_t[BPiece][i] == ROOK) score_WHITE += 500;
			else if (board_t[BPiece][i] == PAWN) score_WHITE += 100;
			else;
		} // if
		else { // BLACK
			if (board_t[BPiece][i] == QUEEN) score_BLACK += 900;
			else if (board_t[BPiece][i] == BISHOP) score_BLACK += 300;
			else if (board_t[BPiece][i] == KNIGHT) score_BLACK += 300;
			else if (board_t[BPiece][i] == ROOK) score_BLACK += 500;
			else if (board_t[BPiece][i] == PAWN) score_BLACK += 100;
			else;
		} // else
	} // for

	return score_WHITE - score_BLACK;
} // int