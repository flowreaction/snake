#include "typedefs.h"
#include "symConst.h"

#ifndef structs_h
#define structs_h
typedef struct highScoreTree {
	UINT score;											//payload
	UCHAR name[NAMELENGTH];								//payload
	struct highScoreTree *higher;						//pointer to higher score
	struct highScoreTree *lower;						//pointer to lower score
}hScore;
#endif // !structs_h

