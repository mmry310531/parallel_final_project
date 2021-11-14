#include <iostream>
#include "Def.h"
#include "DATA.h"
#include "board.h"
using namespace std;



int main()
{
	MoveByte move;
	char s[256];
	int thread_num = 0;
	AUTO = false;
	side = PLAYER_SIDE;
	xside = COMPUTER_SIDE;
	board_init();
	while (true) {
		generateMove();
		if (side == COMPUTER_SIDE || AUTO) {
			// time start;
			//move = search();
			//if (move == NULL) {
			//	printf("no legal move\n");

			//	// initial a board
			//	init();
			//}
			// time end;

			//make(move);
		}

		else {
			printf("command : ");
			cin >> s;

			if (!strcmp(s, "new")) {
				cout << "create a new game\n";
				board_init();
				board_print();
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
				board_print();
			}
			else if (!strcmp(s, "q")) {
				return 0;
			}
			else {
				move = ReadMove(s);
				cout << convertIndex2Readible(move.from) << ", " << convertIndex2Readible(move.to) << endl;

				if (move.legal == NONE) {
					cout << "Illegal Move!\n";
					continue;
				}
				else {
					bool moveLegal = makeMove(move);

					//side = COMPUTER_SIDE;
					//xside = PLAYER_SIDE;
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