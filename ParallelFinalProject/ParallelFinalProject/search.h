#pragma once
#include "Def.h" 
#include <iostream>
#include <string> 

int EvaluateBoard(int board_[2][64]);
int before_search();
int search(int alpha, int beta, int depth);

void ReadBook();

typedef struct StepSet {
public:
    int side;
    int step_num;
    string step;
} StepSet; // StepSet