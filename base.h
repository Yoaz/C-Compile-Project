#include <stdio.h>
#include "data.c"



/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\* 

/* starting a first round fetching lines from file and parse */
int firstRound(FILE *);

/* initiate second round in case first round was clean from errors */
int secondRound(FILE *);