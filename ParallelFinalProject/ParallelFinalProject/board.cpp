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
	castle |= char(15); // four castle ways

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
			else if (board[BPiece][8 * i + j] == ROOK) {
				if (board[BColor][8 * i + j] == WHITE) {
					cout << "R";
				}
				else {
					cout << "r";
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
	mb.from = mb.to = mb.promote =  NONE; mb.legal = false;

	if (s.length() < 4) {
		return mb;
	}


	if ((s[0] >= 'a' && s[0] <= 'h') && (s[1] >= '1' && s[1] <= '8') && (s[2] >= 'a' && s[2] <= 'h') && (s[3] >= '1' && s[3] <= '8'))
	{
		from = s[0] - 'a';
		from += 8 * (8 - (s[1] - '0'));
		to = s[2] - 'a';
		to += 8 * (8 - (s[3] - '0'));
		promote = -1;
		//cout << convertIndex2Readible(from) << ", " << convertIndex2Readible(to) << endl;
		piece = board[BPiece][from];
		if (s.length() == 5) {
			if (s[4] == 'r') {
				promote = ROOK;
			}
			else if (s[4] == 'b') {
				promote = BISHOP;
			}
			else if (s[4] == 'n') {
				promote = KNIGHT;
			}
			else{
				promote = QUEEN;
				// shouldn't be here
			}
		}

		//cout << "first Move Num " << first_move[1] << endl;
		for (int i = 0; i < first_move[1]; ++i) {
			cout << convertIndex2Readible(gen_dat[i].movebyte.from) << ", " << convertIndex2Readible(gen_dat[i].movebyte.to) << endl;
			if (gen_dat[i].movebyte.from == from && gen_dat[i].movebyte.to == to) {

				mb.from = from;
				mb.to = to;
				mb.promote = promote;
				mb.legal = 1;
				return mb;
			}
		}
	}
	return mb;
	
}

bool makeMove(MoveByte moveByte)
{
	int from = moveByte.from;
	int to = moveByte.to;

	bool Pass = false;
	//int piece = board[BPiece][moveByte.from];
	int color = board[BColor][moveByte.from];

	if (color == side) {
		if (!checkLegalMove(moveByte)) {
			return false;
		}
	}
	else {
		return false;
	}



	if (board[BPiece][from] == ROOK) {
		if (from == H1) {
			cout << "hello" << endl;
			castle &= char(14); // 1110
		}
		else if (from == A1) {
			castle &= char(13); // 1101
		}
		else if (from == H8) {
			castle &= char(11); // 1011
		}
		else if (from == A8) {
			castle &= char(7); // 0111
		}
	}
	else if (board[BPiece][from] == KING) {

		if (from == E1) {
			if (moveByte.castle == 1) {
				board[BPiece][H1] = NONE;
				board[BPiece][F1] == ROOK;
			}
			else if (moveByte.castle == 2) {
				board[BPiece][A1] = NONE;
				board[BPiece][D1] == ROOK;
			}
			castle = castle & 12;
		}
		else if (from == E8) {
			if (moveByte.castle == 4) {
				board[BPiece][H8] = NONE;
				board[BPiece][F8] == ROOK;
			}
			else if (moveByte.castle == 8) {
				board[BPiece][A8] = NONE;
				board[BPiece][D8] == ROOK;
			}
			castle = castle & 3;
		}
	}

	board[BPiece][moveByte.to] = board[BPiece][moveByte.from];
	board[BColor][moveByte.to] = side;

	board[BPiece][moveByte.from] = NONE;
	board[BColor][moveByte.from] = NONE;

	return true;
}

void generateMove()
{

	first_move[ply + 1] = first_move[ply];
	PreComputeMove();
	for (int square = 0; square < 64; ++square) {
		if (board[BColor][square] == side) {
			// handle queen | rook | bishop
			if (board[BPiece][square] == QUEEN || board[BPiece][square] == ROOK || board[BPiece][square] == BISHOP) {
				int start_n = 0;
				int end_n = 8;
				if (board[BPiece][square] == BISHOP)
					start_n = 4;
				if (board[BPiece][square] == ROOK)
					end_n = 4;
				for (int direction = start_n; direction < end_n; ++direction) {
					//cout << "num " << NumSquaresToEdge[square][direction] << endl;
					for (int n = 0; n < NumSquaresToEdge[square][direction]; ++n) {
						int targetSquare = square + move_offset[direction] * (n + 1);

						if (board[BColor][targetSquare] == side) {
							break;
						}
						//cout << board[BPiece][square] << "  , " << convertIndex2Readible(targetSquare) << endl;

						if (board[BColor][targetSquare] == xside) {
							push_moveable_piece(square, targetSquare, NONE, NONE, true, false, false, false);
							break;
						}
						else {
							push_moveable_piece(square, targetSquare, NONE, NONE, false, false, false, false);
						}


					}
				}
			}

			// handle Knight
			if (board[BPiece][square] == KNIGHT) {
				for (int i = 0; i < 8; i++) {
					int knightSquare = square + knight_jump[i];
					if (knightSquare >= 0 && knightSquare < 64) {
						int knight_col = COL(knightSquare);
						int knight_row = ROW(knightSquare);

						int rX = std::abs(COL(square) - knight_col);
						int rY = std::abs(ROW(square) - knight_row);
						if ((rX + rY) == 3) {
							if (board[BColor][knightSquare] == side)
								continue;
							//cout << board[BPiece][square] << "  , " << convertIndex2Readible(knightSquare) << endl;
							if (board[BColor][knightSquare] == xside) {
								push_moveable_piece(square, knightSquare, false, false, true, false, false, false);
							}
							else {
								push_moveable_piece(square, knightSquare, false, false, false, false, false, false);
							}
						}

					}
				}
			}

			// handle king
			if (board[BPiece][square] == KING) {
				for (int i = 0; i < 8; ++i) {
					int TargetSquare = square + move_offset[i];
					if (TargetSquare >= 0 && TargetSquare < 64 && board[BColor][TargetSquare] != side) {
						if (board[BColor][TargetSquare] == xside) {
							push_moveable_piece(square, TargetSquare, false, false, true, false, false, false);
						}
						else {
							push_moveable_piece(square, TargetSquare, false, false, false, false, false, false);
						}
						//cout << "King" << convertIndex2Readible(square) << ", " << convertIndex2Readible(TargetSquare) << endl;
					}
				}

				// handle castle
				if (side == WHITE) {
					// right
					if (castle & char(1)) {
						//if(attack ( F1, G1) continue
						//else
						cout << "check" << endl;
						cout << (castle+'0') << endl;
						if (board[BColor][F1] == NONE && board[BColor][G1] == NONE) {
							push_moveable_piece(square, G1, false, 1, false, false, false, false);
							//cout << board[BPiece][square] << "  , " << convertIndex2Readible(G1) << endl;
						}
					}
					// left
					if (castle & char(2)) {
						//if(attack ( C1, D1) continue
						//else
						if (board[BColor][C1] == NONE && board[BColor][D1] == NONE) {
							push_moveable_piece(square, C1, false, 2, false, false, false, false);
							//cout << board[BPiece][square] << "  , " << convertIndex2Readible(C1) << endl;
						}
					}
				}
				else {
					// right
					if (castle & 4) {
						//if(attack ( F7, G7) continue
						//else
						if (board[BColor][F7] == NONE && board[BColor][G7] == NONE) {
							push_moveable_piece(square, G7, false, 4, false, false, false, false);
							//cout << board[BPiece][square] << "  , " << convertIndex2Readible(G7) << endl;
						}
					}
					// left
					if (castle & 8) {

						//if(attack ( C7, D7) continue
						//else
						if (board[BColor][C7] == NONE && board[BColor][D7] == NONE) {
							push_moveable_piece(square, C7, false, 8, false, false, false, false);
							//cout << board[BPiece][square] << "  , " << convertIndex2Readible(C7) << endl;
						}
					}
				}
			}

			// TODO

			// handle pawn

			if (board[BPiece][square] == PAWN) {
				if (side == WHITE) {
					if (COL(square) != 0 && board[BColor][square - 9] == BLACK) {
						push_moveable_piece(square, (square - 9), NONE, NONE, true, false, true, false);
					}
					if (COL(square) != 7 && board[BColor][square - 7] == BLACK) {
						push_moveable_piece(square, (square - 7), NONE, NONE, true, false, true, false);
					}

					if (board[BColor][square - 8] == NONE) {
						push_moveable_piece(square, (square - 8), NONE, NONE, false, false, true, false);
						if (board[BPiece][square - 16] && square >= 48) {
							push_moveable_piece(square, (square - 16), NONE, NONE, false, false, true, true);
						}
					}

				}
				else {
					if (COL(square) != 0 && board[BColor][square + 7] == WHITE) {
						push_moveable_piece(square, (square + 7), NONE, NONE, true, false, true, false);
					}
					if (COL(square) != 7 && board[BColor][square + 9] == WHITE) {
						push_moveable_piece(square, (square + 9), NONE, NONE, true, false, true, false);
					}

					if (board[BColor][square + 8] == NONE) {
						push_moveable_piece(square, (square + 8), NONE, NONE, false, false, true, false);
						if (board[BPiece][square + 16] && square <= 15) {
							push_moveable_piece(square, (square + 16), NONE, NONE, false, false, true, true);
						}
					}

				}

			}

		}
	}
}
//	typedef struct MoveByte{
//	int from;
//	int to;
//	int piece;
//	int promote;
// 0 is can castle ; 1 is left ; 2 is right ; -1 can't castle
//	int castle;
//	bool capture;
//	bool en_capture;
//	bool IsPromote;
//	int legal;
//} MoveByte;
//
void push_moveable_piece(int from, int to, int promote, int castle, bool capture, bool en_capture, bool pawn, bool pawn2) {

	// white pawn move to promote
	if (to < 8 && pawn) {

	}
	// black pawn move to promote
	if (to >= 56 && pawn) {

	}

	MoveByte_set* g = &gen_dat[first_move[ply+1]++];
	g->movebyte.from = from;
	g->movebyte.to = to;
	g->movebyte.castle = castle;
	g->movebyte.promote = NONE;
	g->movebyte.legal = true;
	g->score = 0;
	g->score = 1000000 + (board[BPiece][to] * 10) - board[BPiece][from];
}




bool checkLegalMove(MoveByte movebyte) {


	// attack
	if (false) {

	}



	return true;
}


bool backMove()
{
	return true;
}

void PreComputeMove() {
	for (int file = 0; file < 8; ++file) {
		for (int rank = 0; rank < 8; ++rank) {
			int numNorth = file;
			int numSouth = 7 - file;
			int numWest = rank;
			int numEast = 7 - rank;

			int square = file * 8 + rank;
			int col = COL(square);
			int row = ROW(square);
			NumSquaresToEdge[square][0] = numNorth;
			NumSquaresToEdge[square][1] = numSouth;
			NumSquaresToEdge[square][2] = numWest;
			NumSquaresToEdge[square][3] = numEast;
			NumSquaresToEdge[square][4] = (numNorth < numWest) ? numNorth : numWest;
			NumSquaresToEdge[square][5] = (numSouth < numEast) ? numSouth : numEast;
			NumSquaresToEdge[square][6] = (numNorth < numEast) ? numNorth : numEast;
			NumSquaresToEdge[square][7] = (numSouth < numWest) ? numSouth : numWest;


		}

	}

}

// for debug
string convertIndex2Readible(int square)
{
	string square_s = "";
	int col = COL(square);
	int row = ROW(square);

	switch (col) {
	case 0:
		square_s += "A";
		break;
	case 1:
		square_s += "B";
		break;
	case 2:
		square_s += "C";
		break;
	case 3:
		square_s += "D";
		break;
	case 4:
		square_s += "E";
		break;
	case 5:
		square_s += "F";
		break;
	case 6:
		square_s += "G";
		break;
	case 7:
		square_s += "H";
		break;
	}
	switch (row) {
	case 0:
		square_s += "8";
		break;
	case 1:
		square_s += "7";
		break;
	case 2:
		square_s += "6";
		break;
	case 3:
		square_s += "5";
		break;
	case 4:
		square_s += "4";
		break;
	case 5:
		square_s += "3";
		break;
	case 6:
		square_s += "2";
		break;
	case 7:
		square_s += "1";
		break;
	}

	return square_s;
}