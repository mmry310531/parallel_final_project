#include <iostream>
#include "Def.h"
#include "DATA.h"
#include "board.h"
#include "search.h"
#include <time.h>
#include <omp.h>
#include <vector>
using namespace std;
vector<int> AllTotalnode;
vector<clock_t> AllClock;

int main()
{
	omp_set_num_threads(1);
	totalNodes = 0;
	MoveByte move;
	char s[256];
	thread_num = 4;
	string s_computer = "";
	int whichBook, temp_ply;
	AUTO = true ;
	side = PLAYER_SIDE;
	xside = COMPUTER_SIDE;
	board_init();
	generateMove(false);
	ReadBook();
	while (true) {
		
	if (side == COMPUTER_SIDE || AUTO) {
			 if (side == WHITE) {
				 cout << "white move \n";
			 }
			 else {
				 cout << "black move \n";
			 }

			 temp_ply = hply - 1;
			 if (temp_ply == 0) whichBook = searchBook("NONE", temp_ply);
			 else if (AUTO) whichBook = searchBook(s_computer, temp_ply);
			 else whichBook = searchBook(s, temp_ply);

			 //cout << "whichBook: " << whichBook + 1 << endl;

			 if (whichBook + 1)
				 s_computer = getStep(whichBook, temp_ply).c_str();
		
			 clock_t startTime = 0;
			 //if (whichBook + 1 == 0) {
			 if (true) {

				 startTime = clock();
				 before_search();
				 clock_t endTime = clock() - startTime;
				 cout << "search time : " << endTime << endl;
				 cout << "total Nodes : " << node << endl;
				 AllTotalnode.push_back(totalNodes);
				 AllClock.push_back(endTime);


				 bool legalMove = makeMove(pv[0][0]);
				 if (!legalMove) {
					 AUTO = false;
					 side = PLAYER_SIDE;
					 cout << "game over\n";
					 break;
				 }
				 else {
					 cout << convertIndex2Readible(pv[0][0].from) << ", " << convertIndex2Readible(pv[0][0].to) << endl;
				 }

				 cout << "hply :  " << hply << endl;
				 if (hply == 30) break;

				 ply = 0;

				 
				 generateMove(false);
				 
				 board_print(board);
				 //board_print_color(board);
				 continue;
			 }
			 else { // s : first c2c4
				 move = ReadMove(s_computer);
				 bool moveLegal = makeMove(move);
				 if (moveLegal) {
				 }
				 else {
					 cout << "Illegal Move!\n";
				 }
				 ply = 0;
				 generateMove(false);
				 board_print(board);
				 if (AUTO) continue;
			 }
			 

		 }

		if ( 1 ) {
			printf("command : ");
			cin >> s;

			if (!strcmp(s, "new")) {
				cout << "create a new game\n";
				board_init();
				generateMove(false);
				board_print(board);
			}
			else if (!strcmp(s, "t")) {
				cin >> thread_num;
			}
			else if (!strcmp(s, "s")) {
				thread_num = 1;
			}
			else if (!strcmp(s, "help")) {
				printf("new : restart a new game\n");
				printf("t ${number} : decide therad numbers\n");
				printf("s : change to serial search\n");
				printf("how to make move ? input [a-h][1-8][a-h][1-8]");
				printf("	the first two parameter decide where the piece come FROM\n");
				printf("	the last two parameter decide where the piece go TO\n");
				printf("	If you want promote, add type at final character\n");
				printf("    example: A4A5 \n");
				printf("             A7A8Q \n");
			}
			else if (!strcmp(s, "p")) {
				board_print(board);
				cout << "score : " << EvaluateBoard(board) << endl;
			}
			else if (!strcmp(s, "q")) {
				return 0;
			}
			else if (!strcmp(s, "u")) {
				if (hply != 0) {
					backMove();
					generateMove(false);
				}
			}
			else if (!strcmp(s, "check")) {
				for (int i = 0; i < hply; i++) {
					board_print(history[i].board);					
				}
			}
			else {
				move = ReadMove(s);
				if (!move.legal) {
					cout << "Illegal Move!\n";
					continue;
				}
				else {
					bool moveLegal = makeMove(move);
					if (moveLegal) {
						//side ^= 1;
						//xside ^= 1;
					}
					else {
						cout << "Illegal Move!\n";
					}
					ply = 0;
					generateMove(false);
				}
			}
			
			//if (move == -1 || !make_move(move)) {
			//	printf("illegal move\n");
			//}
			//else {
			//	printf_result();
			//	init();
			//}

		}

	}

	
	//cout << "----------------Time-------------\n";
	//for (int i = 0; i < AllClock.size(); i++) {
	//	cout << AllClock[i] << endl;
	//} // for
	//cout << "\n----------------Total Nodes-------------\n";
	//for (int i = 0; i < AllTotalnode.size(); i++) {
	//	cout << AllTotalnode[i] << endl;
	//} // for
	if (AllTotalnode.size() == AllClock.size()) {
		cout << "herllo " << endl;
	}
	double TimePerNodes_total = 0;
	int totalNODEs = 0;
	clock_t clock= 0;
	cout << clock << endl;
	cout << "-----------avg hply per time-----\n";
	for (int i = 0; i < AllTotalnode.size(); ++i) {
		cout << "hply : " << i << "spend avg: " << (double)(AllTotalnode[i]) / (double)(AllClock[i]) << endl;
		cout << "finds nodes :" << (AllTotalnode[i]) << endl;
		cout << "spend time " << AllClock[i] << endl;
		totalNODEs += (AllTotalnode[i]);
		clock += (AllClock[i]);
		TimePerNodes_total += (double)(AllTotalnode[i]) / (double)(AllClock[i]);
	}
	cout << "total : " << TimePerNodes_total / (AllTotalnode.size()-1) << "\n"; // per ply
	cout << "total nodes: " << totalNODEs << endl;
	cout << "total time: " << clock << endl;
	cout << "test: " << (double)totalNODEs / (double)clock << endl;
	return 0;



}