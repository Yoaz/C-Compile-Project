#include <stdio.h>
#include <string.h>
#include <ctype.h> /* for isalnum() etc */

#include "globals.h"
#include "errors.h"


extern numOfErrors; 
extern spLine *pSpLine;
extern struct instructions ICS[NUM_INSTRUCTION_COMMANDS];


/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */

/* general */
boolean whiteCh(char);
void resetSpLine(spLine *);
boolean relevantToCheck(char *);
boolean addArgToArgList(char *);

/* label */
boolean fetchLabel(char *);
boolean isLabel(char *);
boolean isLegitLabel(char *);
boolean isInstType(char *);
boolean isDirType(char *);

/* directive */
boolean parseString(char *);
boolean parseData(char *);
boolean parseExternEntry(char *);
boolean legitDataArg(char *);

/* instruction */
boolean isNumber(char *arg);
boolean parseInst(char *);
boolean legitInstArg(char *);
boolean numInRange(char *);
int getInstructionI(char *);
boolean comptInstArg(char *);
int instArgsCount(int);
boolean isReg(char *);
boolean fetchArgs(char *);
boolean legitNumInstArgs();