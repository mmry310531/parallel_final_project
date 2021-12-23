#include "DATA.h"
#include "board.h"
#include "Def.h"

extern __declspec(thread) int board[2][64];
extern __declspec(thread) Hist_t history[1024];
extern __declspec(thread) int side, xside, ep, ply, hply;
extern __declspec(thread) char castle;
extern __declspec(thread) MoveByte_set gen_dat[4096];
extern __declspec(thread) int first_move[32];
extern __declspec(thread) MoveByte pv[32][32];
extern __declspec(thread) int pv_length[32];


int board_init()
{
	side = WHITE;
	ply = 0;
	hply = 0;
	castle |= char(15); // four castle ways

	for (int i = 0; i < 64; ++i) {
		board[BColor][i] = init_color[i];
		board[BPiece][i] = init_piece[i];
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 64; j++) {
			history[hply].board[i][j] = board[i][j];
			history[hply].castle = castle;
			//history[hply].ep = ep;
		}
	}
	hply++;
	return 0;
}

int board_print(int board_[2][64])
{
	cout << "    A  B  C  D  E  F  G  H\n\n";
	for (int i = 0; i < 8; ++i) {
		cout << (8 - i) << "   ";
		for (int j = 0; j < 8; ++j) {
			if (board_[BPiece][8 * i + j] == KING) {
				if (board_[BColor][8 * i + j] == WHITE) {
					cout << "K";
				}
				else {
					cout << "k";
				}
			}
			else if (board_[BPiece][8 * i + j] == QUEEN) {
				if (board_[BColor][8 * i + j] == WHITE) {
					cout << "Q";
				}
				else {
					cout << "q";
				}
			}
			else if (board_[BPiece][8 * i + j] == BISHOP) {
				if (board_[BColor][8 * i + j] == 0) {
					cout << "B";
				}
				else {
					cout << "b";
				}
			}
			else if (board_[BPiece][8 * i + j] == KNIGHT) {
				if (board_[BColor][8 * i + j] == WHITE) {
					cout << "N";
				}
				else {
					cout << "n";
				}
			}
			else if (board_[BPiece][8 * i + j] == ROOK) {
				if (board_[BColor][8 * i + j] == WHITE) {
					cout << "R";
				}
				else {
					cout << "r";
				}
			}
			else if (board_[BPiece][8 * i + j] == PAWN) {
				if (board_[BColor][8 * i + j] == WHITE) {
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
	cout << "\n    A  B  C  D  E  F  G  H\n";
	return 0;
}

MoveByte ReadMove(string s) {
	MoveByte mb;
	int from, to, piece, promote, color;
	mb.from = mb.to = mb.promote = NONE; mb.legal = false; mb.pawn2 = false;

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
		//
		piece = board[BPiece][from];
		color = board[BColor][from];
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
			else {
				promote = QUEEN;
				// shouldn't be here
			}
		}
		// for debug
		//for (int i = 0; i < first_move[1]; ++i) {
		//	cout << convertIndex2Readible(gen_dat[i].movebyte.from) << ", " << convertIndex2Readible(gen_dat[i].movebyte.to) << endl;
		//}
		//cout << "first Move Num " << first_move[1] << endl;
		// for (int i = 0; i < first_move[1]; ++i) {
		// 	cout << convertIndex2Readible(from) << ", " << convertIndex2Readible(to) << endl;
		// }
		//cout << "first Move Num " << first_move[1] << endl;
		for (int i = 0; i < first_move[1]; ++i) {
			// cout << convertIndex2Readible(from) << ", " << convertIndex2Readible(to) << endl;
			if (gen_dat[i].movebyte.from == from && gen_dat[i].movebyte.to == to) {

				mb.from = from;
				mb.to = to;
				mb.promote = promote;
				mb.castle = gen_dat[i].movebyte.castle;
				mb.en_capture = gen_dat[i].movebyte.en_capture;
				mb.legal = 1;
				mb.pawn2 = false;

				if (piece == PAWN && (from - to == 16))
					mb.pawn2 = true;
				if (piece == PAWN && (to - from == 16))
					mb.pawn2 = true;
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
	int ep;
	if (hply != 0) ep = history[hply - 1].ep;

	if (board[BPiece][from] == ROOK) {
		if (from == H1) {
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
	//else if (board[BPiece][from] == KING) {
	else if (moveByte.castle != false) {
		if (from == E1) {
			if (moveByte.castle == 1) {
				if (in_check(side)) return false;
				if (attack(F1, xside) || attack(G1, xside) || board[BColor][F1] != NONE || board[BColor][G1] != NONE) return false;
				board[BPiece][F1] = ROOK;
				board[BPiece][H1] = NONE;
				board[BColor][H1] = NONE;
				board[BColor][F1] = side;
			}
			else if (moveByte.castle == 2) {
				if (in_check(side)) return false;
				if (attack(C1, xside) || attack(D1, xside) || board[BColor][C1] != NONE || board[BColor][D1] != NONE) return false;
				board[BPiece][A1] = NONE;
				board[BPiece][D1] = ROOK;
				board[BColor][A1] = NONE;
				board[BColor][D1] = side;
			}
			castle = castle & 12;  // 1100
		}
		else if (from == E8) {
			if (moveByte.castle == 4) {
				if (in_check(side)) return false;
				if (attack(F8, xside) || attack(G8, xside) || board[BColor][F8] != NONE || board[BColor][G8] != NONE) return false;
				board[BPiece][H8] = NONE;
				board[BPiece][F8] = ROOK;
				board[BColor][H8] = NONE;
				board[BColor][F8] = side;
			}
			else if (moveByte.castle == 8) {
				if (in_check(side)) return false;
				if (attack(C8, xside) || attack(D8, xside) || board[BColor][C8] != NONE || board[BColor][D8] != NONE) return false;
				board[BPiece][A8] = NONE;
				board[BPiece][D8] = ROOK;
				board[BColor][A8] = NONE;
				board[BColor][D8] = side;
			}
			castle = castle & 3; // 0011
		}
	}

	if (board[BPiece][from] == PAWN && board[BPiece][to] && board[BColor][from] == WHITE && (abs(from - ep) == 1)) {
		board[BPiece][ep] = NONE;
		board[BColor][ep] = NONE;
	} // if : en passant move

	if (board[BPiece][from] == PAWN && board[BPiece][to] == NONE && board[BColor][from] == BLACK && (abs(from - ep) == 1)) {
		board[BPiece][ep] = NONE;
		board[BColor][ep] = NONE;
	} // if

	if (moveByte.promote > 1) {
		board[BPiece][moveByte.to] = moveByte.promote;
	}
	else {
		board[BPiece][moveByte.to] = board[BPiece][moveByte.from];
	}

	board[BColor][moveByte.to] = side;

	board[BPiece][moveByte.from] = NONE;
	board[BColor][moveByte.from] = NONE;


	// backup history
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 64; j++) {
			history[hply].board[i][j] = board[i][j];
		}
	}

	if (moveByte.pawn2) {
		// cout << "en : " << convertIndex2Readible(from) << ", " << convertIndex2Readible(to) << endl;
		history[hply].ep = to;
	} // if				
	else history[hply].ep = -1;

	history[hply].castle = castle;

	side ^= 1;
	xside ^= 1;
	//cout << "side : " << side << "xside : " << xside << endl;
	hply++;
	ply++;
	if (in_check(xside)) {
		backMove();
		return false;
	}

	return true;
}
void generateMove(bool search)
{
	//cout << "side " << side << "ply : " << ply << endl;
	//board_print(board);
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
							push_moveable_piece(search, square, targetSquare, NONE, NONE, true, false, false, false);
							break;
						}
						else {
							push_moveable_piece(search, square, targetSquare, NONE, NONE, false, false, false, false);
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
								push_moveable_piece(search, square, knightSquare, false, false, true, false, false, false);
							}
							else {
								push_moveable_piece(search, square, knightSquare, false, false, false, false, false, false);
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
						int TargetCol = COL(TargetSquare);
						int TargetRow = ROW(TargetSquare);
						int c = std::abs(COL(square) - TargetCol);
						int r = std::abs(ROW(square) - TargetRow);
						if (c <= 1 && r <= 1) {
							if (board[BColor][TargetSquare] == xside) {
								push_moveable_piece(search, square, TargetSquare, false, false, true, false, false, false);
							}
							else {
								push_moveable_piece(search, square, TargetSquare, false, false, false, false, false, false);
							}
						}
					}
				}

				// handle castle
				if (side == WHITE) {
					// right
					if (castle & char(1)) {
						//if(attack ( F1, G1) continue
						//else
						if (board[BColor][F1] == NONE && board[BColor][G1] == NONE) {
							push_moveable_piece(search, square, G1, false, 1, false, false, false, false);
						}
					}
					// left
					if (castle & char(2)) {
						//if(attack ( C1, D1) continue
						//else
						if (board[BColor][C1] == NONE && board[BColor][D1] == NONE) {
							push_moveable_piece(search, square, C1, false, 2, false, false, false, false);
						}
					}
				}
				else {
					// right
					if (castle & 4) {
						//if(attack ( F7, G7) continue
						//else
						if (board[BColor][F7] == NONE && board[BColor][G7] == NONE) {
							push_moveable_piece(search, square, G7, false, 4, false, false, false, false);
						}
					}
					// left
					if (castle & 8) {

						//if(attack ( C7, D7) continue
						//else
						if (board[BColor][C7] == NONE && board[BColor][D7] == NONE) {
							push_moveable_piece(search, square, C7, false, 8, false, false, false, false);
						}
					}
				}
			}

			// TODO

			// handle pawn
			int ep = history[hply - 1].ep;

			if (board[BPiece][square] == PAWN) {
				if (side == WHITE) {
					if (COL(square) != 0 && board[BColor][square - 9] == BLACK) {
						push_moveable_piece(search, square, (square - 9), NONE, NONE, true, false, true, false);
					}
					if (COL(square) != 7 && board[BColor][square - 7] == BLACK) {
						push_moveable_piece(search, square, (square - 7), NONE, NONE, true, false, true, false);
					}

					if (board[BColor][square - 8] == NONE) {
						push_moveable_piece(search, square, (square - 8), NONE, NONE, false, false, true, false);
						if (board[BPiece][square - 16] == NONE && square >= 48) {
							push_moveable_piece(search, square, (square - 16), NONE, NONE, false, false, true, true);
						}
					}

					if (ep != -1) {
						if (square - 1 == ep) {
							push_moveable_piece(search, square, (square - 9), NONE, NONE, false, true, true, false);
						} // if
						if (square + 1 == ep) {
							push_moveable_piece(search, square, (square - 7), NONE, NONE, false, true, true, false);
						} // if					
					} // if

				}
				else {
					if (COL(square) != 0 && board[BColor][square + 7] == WHITE) {
						push_moveable_piece(search, square, (square + 7), NONE, NONE, true, false, true, false);
					}
					if (COL(square) != 7 && board[BColor][square + 9] == WHITE) {
						push_moveable_piece(search, square, (square + 9), NONE, NONE, true, false, true, false);
					}

					if (board[BColor][square + 8] == NONE) {
						push_moveable_piece(search, square, (square + 8), NONE, NONE, false, false, true, false);
						if (board[BPiece][square + 16] == NONE && square <= 15) {
							push_moveable_piece(search, square, (square + 16), NONE, NONE, false, false, true, true);
						}
					}

					if (ep != -1) {
						if (square - 1 == ep) {
							push_moveable_piece(search, square, (square + 7), NONE, NONE, false, true, true, false);
						} // if
						if (square + 1 == ep) {
							push_moveable_piece(search, square, (square + 9), NONE, NONE, false, true, true, false);
						} // if					
					} // if

				}

			}

		}
	}
}

void push_moveable_piece(bool search, int from, int to, int promote, int castle, bool capture, bool en_capture, bool pawn, bool pawn2) {

	//if (search) {
		// white pawn move to promote
		if (to < 8 && pawn) {
			for (int i = 0; i < 4; i++) {
				MoveByte_set* g = &gen_dat[first_move[ply + 1]++];
				g->movebyte.from = from;
				g->movebyte.to = to;
				g->movebyte.promote = i + 2; // QUEEN 2 BISHOP 3 KNIGHT 4 ROOK 5
				g->movebyte.legal = true;
				g->score = 0;
				g->score = 1000000 + (board[BPiece][to] * 10) - board[BPiece][from];
				// update board
			} // for

			return;
		}
	//}

	// black pawn move to promote
	//if (search) {
		if (to >= 56 && pawn) {
			for (int i = 0; i < 4; i++) {
				MoveByte_set* g = &gen_dat[first_move[ply + 1]++];
				g->movebyte.from = from;
				g->movebyte.to = to;
				g->movebyte.promote = i + 2; // QUEEN 2 BISHOP 3 KNIGHT 4 ROOK 5
				g->movebyte.legal = true;
				g->score = 0;
				g->score = 1000000 + (board[BPiece][to] * 10) - board[BPiece][from];
				// update board
			} // for

			return;
		}
	//}

	if (search) {
		if (capture == true) {
			MoveByte_set* g = &gen_dat[first_move[ply + 1]++];
			g->movebyte.from = from;
			g->movebyte.to = to;
			g->movebyte.castle = castle;
			g->movebyte.promote = NONE;
			g->movebyte.legal = true;
			g->score = 0;
			g->score = 1000000 + (board[BPiece][to] * 10) - board[BPiece][from];
		}
	}
	else {
		MoveByte_set* g = &gen_dat[first_move[ply + 1]++];
		g->movebyte.from = from;
		g->movebyte.to = to;
		g->movebyte.castle = castle;
		g->movebyte.promote = NONE;
		g->movebyte.legal = true;
		g->score = 0;
		g->score = 1000000 + (board[BPiece][to] * 10) - board[BPiece][from];
	}
	
}

bool backMove()
{
	//debug
	hply -= 2;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 64; j++) {
			board[i][j] = history[hply].board[i][j];
		}
	}
	
	castle = history[hply].castle;
	ep = history[hply].ep;

	side ^= 1;
	xside ^= 1;
	hply++;
	ply--;
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

bool in_check(int s) {
	for (int i = 0; i < 64; i++) {
		if (board[BColor][i] == s && board[BPiece][i] == KING)
			return attack(i, s ^ 1);
	}
	return true;
}

bool attack(int square, int x) {
	PreComputeMove(); //need this one?

	for (int i = 0; i < 64; i++) {
		if (board[BColor][i] == x) {
			//

			if (board[BPiece][i] == PAWN) {
				if (side == WHITE) {
					if (COL(i) != 0 && i - 9 == square) {
						return true;
					}
					if (COL(i) != 7 && i - 7 == square) {
						return true;
					}
				}
				else {
					if (COL(i) != 0 && i + 7 == square) {
						return true;
					}
					if (COL(i) != 7 && i + 9 == square) {
						return true;
					}
				}
			}
			// Done : queen bishop rook 
			else if (board[BPiece][i] == QUEEN ||
				board[BPiece][i] == BISHOP ||
				board[BPiece][i] == ROOK) {
				int start_n = 0;
				int end_n = 8;
				if (board[BPiece][i] == BISHOP)
					start_n = 4;
				if (board[BPiece][i] == ROOK)
					end_n = 4;
				for (int direction = start_n; direction < end_n; ++direction) {
					for (int n = 0; n < NumSquaresToEdge[i][direction]; ++n) {
						int targetSquare = i + move_offset[direction] * (n + 1);
						if ((board[BColor][targetSquare] == 0 || board[BColor][targetSquare] == 1) && targetSquare != square) break;
						if (targetSquare == square) {
							return true;
						}
					}
				}
			}
			else if (board[BPiece][i] == KING) {
				for (int direction = 0; direction < 8; direction++) {
					int targetSquare = i + move_offset[direction];
					if (targetSquare == square) {
						return true;
					}
				}
			}
			else if (board[BPiece][i] == KNIGHT) {
				for (int j = 0; j < 8; j++) {
					int knightSquare = i + knight_jump[j];
					if (knightSquare >= 0 && knightSquare < 64) {
						int knight_col = COL(knightSquare);
						int knight_row = ROW(knightSquare);

						int rX = std::abs(COL(i) - knight_col);
						int rY = std::abs(ROW(i) - knight_row);
						if ((rX + rY) == 3) {
							if (knightSquare == square)
								return true;
						}
					}
				}
			}
		}
	}
	return false;
}


// for debug
string convertIndex2Readible(int square)
{
	string square_s = "";
	int col = COL(square);
	int row = ROW(square);

	switch (col) {
	case 0:
		square_s += "a";
		break;
	case 1:
		square_s += "b";
		break;
	case 2:
		square_s += "c";
		break;
	case 3:
		square_s += "d";
		break;
	case 4:
		square_s += "e";
		break;
	case 5:
		square_s += "f";
		break;
	case 6:
		square_s += "g";
		break;
	case 7:
		square_s += "h";
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