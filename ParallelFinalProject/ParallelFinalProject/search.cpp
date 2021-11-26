#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>

#include "board.h"

#include "search.h"
#include "DATA.h"

vector<StepSet> oneGame;
vector<GameSet> allGame;

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
int searchBook(string s, int hply) { // s : white me c2c4
	vector<int> gotGame;
	int index = 2 * hply;
	for (int i = 0; i < allGame.size(); i++) {
		if (index >= allGame[i].thisGame.size()) {
			allGame[i].got = false;
			continue;
		} // if 

		if (allGame[i].thisGame[index].side == 0) { // must
			string temp_step = allGame[i].thisGame[index].step;
			// cout << "Game" << i << " index: " << temp_step << endl;
			if (allGame[i].got && s == temp_step )
				gotGame.push_back(i);
			else 
				allGame[i].got = false;
		} // if
		else 
			cout << "got error" << endl;
	} // for

	if (!gotGame.empty()) {
		srand(time(NULL));
		int rn = rand() % gotGame.size();
		return gotGame[rn];
	} // if
	else return -1;
} // searchBook()

string getStep(int whichBook, int hply) {
	int index = 2 * hply + 1;
	if (index >= allGame[whichBook].thisGame.size()) return "END";

	string replyMove = allGame[whichBook].thisGame[index].step;

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

int before_search( ) {
	return 0;
}

int quiesceneceSearch(int alpha, int beta) {
	MoveByte bestMove;
	bool cutoff = false;
	int score = EvaluateBoard(history[hply].board);
	if (score > beta) return beta;
	if (score > alpha) alpha = score;
	generateMove(true);

	for (int i = 0; i < first_move[ply]; i < first_move[ply + 1], i++) {
		if (cutoff || !makeMove(gen_dat[i].movebyte))
			continue;
		score = -quiesceneceSearch(-beta, -alpha);
		backMove();
		if (score > alpha && !cutoff) {
			// too good
			if (score >= beta) {
				cutoff = true;
			}
			else {
				alpha = score;
			}
		}
	}
	return alpha;

}


// pvs
int search(int alpha, int beta, int depth) {

	//	draw
	// if(hply != 0){
	//		if(three times same board){
	//			return 0
	//
	//

	if (depth == 0) {
		return quiesceneceSearch(alpha, beta);
	}

}