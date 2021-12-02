#include <iostream>
#include "Def.h"
#include "DATA.h"
#include "board.h"
#include "search.h"
using namespace std;



int main()
{
	MoveByte move;
	char s[256];
	int thread_num = 0;
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

			 cout << "whichBook: " << whichBook + 1 << endl;

			 if (whichBook + 1)
				 s_computer = getStep(whichBook, temp_ply).c_str();


			 if (whichBook + 1 == 0) {
				 cout << "castle: " << (int)castle << endl;
				 before_search();

				 if (!makeMove(pv[0][0])) {
					 AUTO = false;
					 side = PLAYER_SIDE;
					 cout << "game over\n";
				 }

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

	return 0;



}