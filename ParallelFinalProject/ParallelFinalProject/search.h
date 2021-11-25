#pragma once
#pragma warning
#include "Def.h"
#include <iostream>

int EvaluateBoard(int board_[2][64]);
int before_search();
int search(int alpha, int beta, int depth);