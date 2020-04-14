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


void resetGlobals();
void initiate(char *);
boolean firstRound(FILE *);
boolean secondRound(fileObject *);