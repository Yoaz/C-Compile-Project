#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "errors.h"


extern numOfErrors; /* global file error counter */
extern spLine *pSpLine;


/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */

/* The function checks wether a given char is a white char (exclude '\n') */
boolean whiteCh(char);

/* The function checks wether a given char is a white char (exclude '\n') */
boolean relevantToCheck(char *);

/* checks if a given lable string is not a saved word of assembly */
boolean isSavedWord(char *);

/* checks if a given lable is valid string syntax */
boolean isLegitSymbol(char *);

/* will compute and check instructions */
void parseInstruction();

void parseDirective();

void fetchLable(char *);

void resetSpLine(spLine *);

boolean isLabel(char *);

boolean isLegitLabel(char *);
