#include "stdio.h"


int main()
{
	char s[256];
	int tread_num = 0;
	while(true){


		if(side == COMPUTER_SIDE || AUTO){
			// time start;
			move = search();
			if(move == NULL){
				printf("no legal move\n");

				// initial a board
				init();
			}
			// time end;

			make(move);
		}

		else{
			printf("command : ");

			if (scanf("%s", s) == EOF)
				return 0;
			if(!strcmp(s, "new")){
				init();
			}
			if(!strcmp(s, "t")){
				scanf("%d", &thread_num)
			}
			if(!strcmp(s,"s")){
				thread_num = 1;
			}
			if(!strcmp(s, "help")){
				printf("new : restart a new game\n");
				printf("t ${number} : decide therad numbers\n");
				printf("s : change to serial search\n");
				printf("how to make move ? input [a-h][1-8][a-h][1-8]");
				printf("	the first two parameter decide where the piece come FROM\n");
				printf("	the last two parameter decide where the piece go TO\n");
			}
			move = parse_move(s);
			
			if(move == -1 || !make_move(move)){
				printf("illegal move\n");
			}
			else{
				printf_result();
				init();
			}

		}

	}

	return 0;



}