#include "DATA.h"


int board_init()
{
	for (int i = 0; i < 64; ++i) {
		board[BColor][i] = init_color[i];
		board[BPiece][i] = init_piece[i];
	}
	
}