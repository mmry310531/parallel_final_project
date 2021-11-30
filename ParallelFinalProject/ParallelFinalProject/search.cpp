#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "board.h"
#include "search.h"
#include "DATA.h"
vector<StepSet> oneGame;
vector<vector<StepSet>> allGame;

void ReadBook() {
	ifstream tempin;
	tempin.open("book.txt");

	string read_line;
	string token;


	while (getline(tempin, read_line)) {
		if (read_line == "") continue;

		vector<string> arr;
		istringstream delim(read_line);
		while (getline(delim, token, ' ')) {
			arr.push_back(token);
		} // 

		for (int i = 0; i < arr.size(); i++) {
			StepSet s1;
			s1.side = i % 2;
			s1.step_num = i / 2;
			s1.step = arr[i].c_str();
			oneGame.push_back(s1);
		} // for

		allGame.push_back(oneGame);
	} // while

	//for (int i = 0; i < allGame.size(); i++) {
	//	for (int j = 0; j < allGame[i].size(); j++) {
	//		cout << "side : " << allGame[i][j].side << endl;
	//		cout << "step_num : " << allGame[i][j].step_num << endl;
	//		cout << "from : " << allGame[i][j].step << endl;
	//	} // for
	//} // for


} // ReadBook()
int EvaluateBoard(int board_t[2][64]) {
	int score_WHITE = 0;
	int score_BLACK = 0;
	for (int i = 0; i < 64; i++) {
		if (board_t[BColor][i] == WHITE) {
			if (board_t[BPiece][i] == QUEEN) { score_WHITE += 900; }
			else if (board_t[BPiece][i] == BISHOP) { score_WHITE += 300; score_WHITE += bishop_score[i]; }
			else if (board_t[BPiece][i] == KNIGHT) { score_WHITE += 300; score_WHITE += knight_score[i]; }
			else if (board_t[BPiece][i] == ROOK) { score_WHITE += 500; score_WHITE += rook_score[i]; }
			else if (board_t[BPiece][i] == PAWN) { score_WHITE += 100; score_WHITE += pawn_score[i]; }
			else if (board_t[BPiece][i] == KING) { score_WHITE += king_score[i]; }
			else;
		} // if
		else { // BLACK
			if (board_t[BPiece][i] == QUEEN) { score_BLACK += 900;}
			else if (board_t[BPiece][i] == BISHOP) { score_BLACK += 300; score_BLACK += bishop_score[flip[i]]; }
			else if (board_t[BPiece][i] == KNIGHT) { score_BLACK += 300; score_BLACK += knight_score[flip[i]]; }
			else if (board_t[BPiece][i] == ROOK) { score_BLACK += 500; score_BLACK += rook_score[flip[i]]; }
			else if (board_t[BPiece][i] == PAWN) { score_BLACK += 100;score_BLACK += pawn_score[flip[i]]; }
			else if (board_t[BPiece][i] == KING) { score_BLACK += king_score[flip[i]]; }
			else;
		} // else


	} // for
	if(side == WHITE)
		return score_WHITE - score_BLACK;
	else
		return score_BLACK - score_WHITE;
} // int

bool cutoff;

int before_search( ) {
	node = 0;
	int score = 0;
	memset(pv, 0, sizeof(pv));
	
	for (int i = 5; i <= 5; ++i) {
		score = search(-99999, 99999, i);
	}


	while (ply)
		backMove();
	printf("node : %d score : %d \n", node, score);
	return 0;
}

int quiesceneceSearch(int alpha, int beta) {
	MoveByte bestMove;
	bool cutoff = false;
	int score = EvaluateBoard(board);
	if (score > beta) return beta;
	if (score > alpha) alpha = score;
	generateMove(true);
	if (node > 1023)
		return score;
	node++;
	for (int i = first_move[ply]; i < first_move[ply + 1]; i++) {
		if (!makeMove(gen_dat[i].movebyte))
			continue;
		score = -quiesceneceSearch(-beta, -alpha);
		backMove();
		if (score > alpha) {
			// too good
			if (score >= beta) {
				return beta;
			}
			else {
				alpha = score;
				// write pv move
				pv[ply][ply] = gen_dat[i].movebyte;
				// loop over the next ply
				for (int next_ply = ply + 1; next_ply < pv_length[ply+1]; next_ply++) {
					pv[ply][next_ply] = pv[ply + 1][next_ply];
				}
				pv_length[ply] = pv_length[ply + 1];
			}
		}
	}
	
	return alpha;

}


// pvs
int search(int alpha, int beta, int depth) {


	if (!depth)
		return quiesceneceSearch(alpha,  beta);
	node++;

	bool NoLegalMove = true;
	bool Check = false;
	int score = 0;
	pv_length[ply] = ply;
	//	draw
	// if(hply != 0){
	//		if(three times same board){
	//			return 0
	//
	//

	if (ply >= (32 - 1))
		return EvaluateBoard(board);
	
	if (in_check(side)) {
		Check = true;
		depth++;
	}
	cutoff = false;
	generateMove(false);
	for (int i = first_move[ply]; i < first_move[ply + 1]; ++i) {
		// sort move to make cutoff condition before
		// sort()
		bool legalMove = makeMove(gen_dat[i].movebyte);
		if (!legalMove)
			continue;
		NoLegalMove = false;
		score = -search(-beta, -alpha, depth-1);
		backMove();
		if (score >= beta)
			return beta;
		if (score > alpha) {
			alpha = score;

			pv[ply][ply] = gen_dat[i].movebyte;

		}
		
	}
	
	if (NoLegalMove) {
		if (Check)
			return -49999 + ply;
		else
			return 0;
	}
	return alpha;


}