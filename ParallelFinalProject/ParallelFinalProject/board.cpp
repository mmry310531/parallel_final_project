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
			else if (board[BPiece][8 * i + j] == ROOK) {
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
		return mb;
	}


	if ((s[0] >= 'a' && s[0] <= 'h') && (s[1] >= '1' && s[1] <= '8') && (s[2] >= 'a' && s[2] <= 'h') &&(s[3] >= '1' && s[3] <= '8'))
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

		cout << "first Move Num " << first_move[1] << endl;
		for (int i = 0; i < first_move[1]; ++i) {
			//cout << convertIndex2Readible(gen_dat[i].movebyte.from) << ", " << convertIndex2Readible(gen_dat[i].movebyte.to) << endl;

			if (gen_dat[i].movebyte.from == from && gen_dat[i].movebyte.to == to) {
				
				mb.from = from;
				mb.to = to;
				mb.piece = piece;
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

	board[BPiece][moveByte.to] = moveByte.piece;
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
			// hangle queen | rook | bishop
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
							int targetSquare = square + move_offset[direction] * (n+1);
							
							if (board[BColor][targetSquare] == side) {
								break;
							}

							MoveByte_set *g =  &gen_dat[first_move[ply]++];
							g->movebyte.from = square;
							g->movebyte.to = targetSquare;
							g->movebyte.piece = NONE;
							g->movebyte.promote = NONE;
							g->movebyte.legal = 0;
							g->score = 0;

							//cout << board[BPiece][square] << "  , " << convertIndex2Readible(targetSquare) << endl;

							if (board[BColor][targetSquare] == xside) {
								g->score = 1000000 + (board[BPiece][targetSquare] * 10) - board[BPiece][square];
								break;
							}
							
						
					}
				}
			}
		}
	}


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
			int numEast = 7 -rank;

			int square = file * 8 + rank;
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