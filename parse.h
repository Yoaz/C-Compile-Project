#include <stdio.h>
#include <string.h>
#include <ctype.h> /* for isalnum() etc */

#include "globals.h"
#include "errors.h"


extern numOfErrors; /* global file error counter */
extern spLine *pSpLine;
extern struct instructions ICS[NUM_INSTRUCTION_COMMANDS];


/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */

boolean fetchLabel(char *);

/* The function checks wether a given char is a white char (exclude '\n') */
boolean whiteCh(char);

/* The function checks wether a given char is a white char (exclude '\n') */
boolean relevantToCheck(char *);

/* checks if a given lable string is not a saved word of assembly */
boolean isInstType(char *);

boolean isDirType(char *);

void resetSpLine(spLine *);

boolean fetchLable(char *);

boolean isLabel(char *);

boolean isNumber(char *arg);

/* checks if a given lable is valid string syntax */
boolean isLegitLabel(char *);

boolean parseExternEntry(char *);

boolean parseString(char *);

boolean addArgToArgList(char *);

boolean parseCmd(char *);

boolean legitInstArg(char *);

boolean numInRange(char *);

int instructionInd(char *);

boolean comptInstArg(char *);

int instArgsCount(int);

boolean isReg(char *);
