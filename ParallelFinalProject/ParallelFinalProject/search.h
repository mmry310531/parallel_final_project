#pragma once
#include "Def.h" 
#include <iostream>
#include <string> 
#include <vector>

int EvaluateBoard(int board_[2][64]);
int before_search();
int quiesceneceSearch(int alpha, int beta);
int search(int alpha, int beta, int depth);

void ReadBook();
int searchBook(string s, int index, bool AUTO);
string getStep(int whichBook, int index, bool AUTO);


typedef struct StepSet {
public:
    int side;
    int step_num;
    string step;
} StepSet; // StepSet

typedef struct GameSet {
public:
    bool got = true;
    vector<StepSet> thisGame;
} GameSet; // StepSet

