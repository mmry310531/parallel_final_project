#include "DATA.h"
#include "board.h"
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