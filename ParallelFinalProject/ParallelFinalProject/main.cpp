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
	while (true) {

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
			if (!strcmp(s, "t")) {
				cin >> thread_num;
			}
			if (!strcmp(s, "s")) {
				thread_num = 1;
			}
			if (!strcmp(s, "help")) {
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
			if (!strcmp(s, "p")) {
				board_print();
			}
			if (!strcmp(s, "q")) {
				return 0;
			}
			else {
				move = ReadMove(s);
				//cout << move.from << ", " << move.to << ", " << move.piece << ", " << move.promote << endl;
				if (move.legal == NONE) {
					continue;
				}
				else {
					bool moveLegal = makeMove(move);
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