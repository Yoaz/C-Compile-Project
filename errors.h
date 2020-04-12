#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdarg.h> /* for va_list, va_args_ etc used in errors.c */

#include "globals.h" /* for all unique program definitions */



extern int numRow;
extern int numColumn;
extern numOfErrors;


/* enum for generic safe memerory allocation functions */
typedef enum{
    sMalloc,
    sCalloc,
    sRealloc
}alloc_t;

/* program errors shortcuts */
typedef enum{NO_FILES=1, FAILED_OPEN, ERRORS_IN_FILE, WRONG_FILES, MISSING_CMD, UNDEF_CMD, LABEL_NUMERIC_START, 
             LABEL_LENGTH, ILLEGAL_LABEL_CHARS, LABEL_EXIST, SAVED_WORD, DIR_NO_FOUND, MISSING_DATA, DATA_INVALID, NOT_WHOLE, 
             MISSING_STRING, MISSING_STRING_START, MISSING_STRING_END, ILLEGAL_BEFORE_STRING, STR_INVALID, 
             LINE_LENGTH, LABEL_ENTRY_EXTERN, MISSING_LBL, MISSING_LBL_DEF, EXTERNAL_LABEL, MISSING_ARG, TOO_MANY_ARGS, TOO_FEW_ARGS,INVALID_INST_ARG, 
             INVALID_REG_REF_SYNT, IMMIDIET_SYNTAX, NUM_RANGE, EXTRA_INPUT, MISSING_ARG_SEP, ILLEGAL_ARG_SEP} errorList;



/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */

void printError(errorList, ...);
void *safeAlloc(alloc_t, ...);

#endif 