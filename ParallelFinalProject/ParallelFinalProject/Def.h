#pragma once
#pragma warning

typedef struct MoveByte{
	int from;
	int to;
	int piece;
	int promote;
	int legal;
} MoveByte;





typedef struct MoveByte_set {
	MoveByte movebyte;
	int score;
} MoveByte_set;


