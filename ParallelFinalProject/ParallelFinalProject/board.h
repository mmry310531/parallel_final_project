#include "Def.h"
#include <iostream>
using namespace std;
int board_init();
int board_print(int board_[2][64]);
MoveByte ReadMove(string s);
bool makeMove(MoveByte movebyte);
void generateMove();
void PreComputeMove();
bool backMove();
bool in_check(int color);
bool attack(int square, int xside);
void push_moveable_piece(int from, int to, int promote, int castle, bool capture, bool en_capture, bool pawn, bool pawn2);
string convertIndex2Readible(int square);
