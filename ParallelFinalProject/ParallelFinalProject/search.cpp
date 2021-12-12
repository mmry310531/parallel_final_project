#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <omp.h>
#include "board.h"
#include "search.h"
#include "DATA.h"
vector<StepSet> oneGame;
vector<GameSet> allGame;
extern __declspec(thread) int board[2][64];
extern __declspec(thread) Hist_t history[1024];
extern __declspec(thread) int side, xside, ep, ply, hply;
extern __declspec(thread) char castle;
extern __declspec(thread) MoveByte_set gen_dat[4096];
extern __declspec(thread) int first_move[32];
extern __declspec(thread) MoveByte pv[32][32];
extern __declspec(thread) int pv_length[32];






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
		oneGame.clear();

        
		for (int i = 0; i < arr.size(); i++) {
			StepSet s1;
			s1.side = i % 2;
			s1.step_num = i / 2;
			s1.step = arr[i].c_str();
			oneGame.push_back(s1);
		} // for

		GameSet g1;
		g1.thisGame = oneGame;
		g1.got = true;
		allGame.push_back(g1);
	} // while

	// for (int i = 0; i < allGame.size(); i++) {
	// 	cout << "Game" << i << ": ";
	// 	for (int j = 0; j < allGame[i].thisGame.size(); j++) {
	// 		cout << allGame[i].thisGame[j].step << " ";
	// 		
	// 	} // for
	// 	cout << endl;

    

} // ReadBook()

int searchBook(string s, int index) {
	vector<int> gotGame;
	int k = index;
    
	for (int i = 0; i < allGame.size(); i++) {
		if (index >= allGame[i].thisGame.size()) {
			allGame[i].got = false;
			continue;
		} // if 

		if (index == 0) gotGame.push_back(i);
		else { // 
			int k = index - 1;
			string temp_step = allGame[i].thisGame[k].step;
			if (allGame[i].got && s == temp_step)
				gotGame.push_back(i);
			else
				allGame[i].got = false;
		} // else
	} // for

	if (!gotGame.empty()) {
		srand(time(NULL));
		int rn = rand() % gotGame.size();
		return gotGame[rn];
	} // if
	else return -1;
} // searchBook()

string getStep(int whichBook, int index) {
	if (index >= allGame[whichBook].thisGame.size()) return "END";
	string replyMove = allGame[whichBook].thisGame[index].step;
	cout << "replyMove : " << replyMove << endl;
#pragma omp parallel for
	for (int i = 0; i < allGame.size(); i++) {
		if (allGame[i].got) {
			if (index >= allGame[i].thisGame.size()) continue;
			if (allGame[i].thisGame[index].step != replyMove) {
				allGame[i].got = false;
			} // if
		} // if
	} // for
	return replyMove;
} // getStep()

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

    // #pragma omp parallel
	// {
	// 	printf("Hello World... from thread = %d\n", omp_get_thread_num());
	// }

} // int


int before_search( ) {
	node = 0;
	int score = 0;
	memset(pv, 0, sizeof(pv));
	int depth = 5;
	// for (int i = 5; i <= 5; ++i) { 

	{
		score = search(-99999, 99999, depth);
	}
		// }


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
// #pragma omp parallel for
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
	}
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
			return beta; // cutoff
		if (score > alpha) {
			alpha = score;

			pv[ply][ply] = gen_dat[i].movebyte;
			// loop over the next ply
// #pragma omp parallel for // 好像會變慢點
			for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
				pv[ply][next_ply] = pv[ply + 1][next_ply];
			}
			pv_length[ply] = pv_length[ply + 1];

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

// pvs
int PVSsearch(int alpha, int beta, int depth) {

	if (!depth)
		return quiesceneceSearch(alpha, beta);
	node++;

	bool bSearchPv = true;
	bool NoLegalMove = true;
	bool Check = false;
	int score = 0;
	bool cutoff = false;
	pv_length[ply] = ply;
	best_pv_length = 0;
	int i, i0;
	int next_ply;
	//	draw
	// if(hply != 0){
	//		if(three times same board){
	//			return 0
	//
	//
	//https://www.chessprogramming.org/Principal_Variation_Search
	if (ply >= (32 - 1))
		return EvaluateBoard(board);

	if (in_check(side)) {
		Check = true;
	}
	
	generateMove(false);

	for (i0 = first_move[ply]; i0 < first_move[ply + 1]; ++i0) {
		if (!makeMove(gen_dat[i0].movebyte))
			continue;
		NoLegalMove = false;
		score = -PVSsearch(-beta, -alpha, depth - 1);
		backMove();
		if (score > alpha) {
			if (score > beta)
				return beta;
			alpha = score;

			best_pv[ply] = pv[ply][ply] = gen_dat[i0].movebyte;
			for (next_ply = ply + 1; next_ply < pv_length[ply + 1]; ++next_ply)
				best_pv[next_ply] = pv[ply][next_ply] = pv[ply + 1][next_ply];
			best_pv_length = pv_length[ply] = pv_length[ply + 1];
		}
		i0++;
		break;
	}

	#pragma omp parallel for schedule(dynamic,1) copyin(board, \
			side, xside, history, ep, castle, ply, hply, \
			gen_dat, first_move, pv, pv_length) \
			private(i, next_ply, score)
	for (i = i0; i < first_move[ply + 1]; ++i) {
		// sort move to make cutoff condition before
		// sort()
		bool legalMove = makeMove(gen_dat[i].movebyte);
		if (!legalMove || cutoff) {
			continue;
		}
		score = -search(-beta, -alpha, depth - 1);

		//if (!legalMove || cutoff)
		//	continue;
		//NoLegalMove = false;
		//if (bSearchPv) {
		//	score = -PVSsearch(-beta, -alpha, depth - 1);
		//}
		//else {
  //          // #pragma omp task
		//	
		//	score = -PVSsearch(-beta, -alpha, depth - 1);
		//	if (score > alpha) {
  // //             // #pragma omp task
		//		score = -PVSsearch(-beta, -alpha, depth - 1);
		//	}
		//}

		backMove();
		#pragma omp critical
		{
			
			if (score > alpha && !cutoff) {
				if (score >= beta)
					cutoff = true;
				else {
					alpha = score;
					bSearchPv = false;


					best_pv[ply] = pv[ply][ply] = gen_dat[i].movebyte;
					//pv[ply][ply] = gen_dat[i].movebyte;
					// loop over the next ply
					// #pragma omp parallel for // 好像會變慢點
					for (next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
						best_pv[next_ply] = pv[ply][next_ply] = pv[ply + 1][next_ply];
					}
					best_pv_length = pv_length[ply] = pv_length[ply + 1];
				}
			}
		}
	}
	if (best_pv_length > 0) {
		pv[ply][ply] = best_pv[ply];
		for (next_ply = ply + 1; next_ply < best_pv_length; ++next_ply)
			pv[ply][next_ply] = best_pv[next_ply];
		pv_length[ply] = best_pv_length;
	}
	if (cutoff) {
		return beta;
	}
	if (NoLegalMove) {
		if (Check)
			return -49999 + ply;
		else
			return 0;
	}
	return alpha;


}