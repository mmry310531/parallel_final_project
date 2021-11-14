#include "Def.h"
#include <iostream>
using namespace std;
int board_init();
int board_print();
MoveByte ReadMove(string s);
bool makeMove(MoveByte movebyte);
bool checkLegalMove(MoveByte movebyte);
void generateMove();
void PreComputeMove();
bool backMove();

string convertIndex2Readible(int square);
