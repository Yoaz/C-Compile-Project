#include <stdio.h>

#include "globals.h"
#include "errors.h"
#include "file.h" /* for use of file defines (ex. READ_ONLY) */
#include "parse.h"
#include "structs.h" /* for use of printLblTable() etc */

extern successFiles;
extern numOfErrors;
extern numColumn;
extern numRow;
extern IC;
extern DC;


/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */

/* reset all global vars */
void resetGlobals();

/* accpets argc and *argv[] from cmd input and initiate assmebler */
void initiate(char *);

/* starting a first round fetching lines from file and parse */
boolean firstRound(FILE *);

/* initiate second round in case first round was clean from errors */
boolean secondRound(FILE *);