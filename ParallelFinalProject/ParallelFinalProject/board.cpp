#include "DATA.h"
#include "board.h"
#include "Def.h"
#include <iostream>
using namespace std;


int board_init()
{
	for (int i = 0; i < 64; ++i) {
		board[BColor][i] = init_color[i];
		board[BPiece][i] = init_piece[i];
	}
	side = WHITE;
	ply = 0;
	half_ply = 0;

	return 0;
}

int board_print()
{
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[BPiece][8 * i + j] == KING) {
				if (board[BColor][8 * i + j] == WHITE) {
					cout << "K";
				}
				else {
					cout << "k";
				}
			}
			else if (board[BPiece][8 * i + j] == QUEEN) {
				if (board[BColor][8 * i + j] == WHITE) {
					cout << "Q";
				}
				else {
					cout << "q";
				}
			}
			else if (board[BPiece][8 * i + j] == BISHOP) {
				if (board[BColor][8 * i + j] == 0) {
					cout << "B";
				}
				else {
					cout << "b";
				}
			}
			else if (board[BPiece][8 * i + j] == KNIGHT) {
				if (board[BColor][8 * i + j] == WHITE) {
					cout << "N";
				}
				else {
					cout << "n";
				}
			}
			else if (board[BPiece][8 * i + j] == CASTLE) {
				if (board[BColor][8 * i + j] == WHITE) {
					cout << "C";
				}
				else {
					cout << "c";
				}
			}
			else if (board[BPiece][8 * i + j] == PAWN) {
				if (board[BColor][8 * i + j] == WHITE) {
					cout << "P";
				}
				else {
					cout << "p";
				}
			}
			else {
				cout << ".";
			}
			if (j != (8 - 1)) {
				cout << "  ";
			}
		}
		cout << "\n";
	}
	return 0;
}

MoveByte ReadMove(string s) {
	MoveByte mb;
	int from, to, piece, promote;
	mb.from = mb.to = mb.piece = mb.promote = mb.legal = NONE;
	if (s.length() < 4) {
		cout << "Illegal Move\n";
		return mb;
	}
	

	if ((s[0] >= 'a' && s[0] <= 'h') &&
		(s[1] >= '1' && s[1] <= '8') &&
		(s[2] >= 'a' && s[2] <= 'h') &&
		(s[3] >= '1' && s[3] <= '8'))
	{
		from = s[0] - 'a';
		from += 8 * (8 - (s[1] - '0'));
		to = s[2] - 'a';
		to += 8 * (8 - (s[3] - '0'));

		piece = board[BPiece][from];
		promote = -1;
		if (s.length() == 5) {
			if (s[4] == 'k') {
				promote = KING;
			}
			else if (s[4] == 'b') {
				promote = BISHOP;
			}
			else if (s[4] == 'n') {
				promote = KNIGHT;
			}
			else {
				promote = 0;
				// shouldn't be here
			}
		}

		mb.from = from;
		mb.to = to;
		mb.piece = piece;
		mb.promote = promote;
		mb.legal = 1;
		return mb;
	}


	else {
		cout << "Illegal Move\n";
		// read error
		return mb;
	}

}

bool makeMove(MoveByte moveByte)
{

	board[BPiece][moveByte.to] = moveByte.piece;
	board[BColor][moveByte.to] = side;

	board[BPiece][moveByte.from] = NONE;
	board[BColor][moveByte.from] = NONE;

	return true;
}

bool backMove()
{
	return true;
}