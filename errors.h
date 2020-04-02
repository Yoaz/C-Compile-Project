#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdarg.h> /* for va_list, va_args_ etc used in errors.c */

#include "globals.h" /* for all unique program definitions */



extern int numRow;
extern int numColumn;


/* enum for generic safe memerory allocation functions */
typedef enum{
    sMalloc,
    sCalloc,
    sRealloc
}alloc_t;

/* program errors shortcuts */
typedef enum{NO_FILES=1, FAILED_OPEN, ERRORS_IN_FILE, WRONG_FILES, MISSING_CMD, UNDEF_CMD, LABEL_NUMERIC_START, 
             LABEL_LENGTH, ILLEGAL_LABEL_CHARS, LABEL_EXIST, SAVED_WORD, DIR_NO_FOUND, MISSING_DATA, DATA_INVALID, MISSING_STRING_START, 
             MISSING_STRING_END, ILLEGAL_BEFORE_STRING, MISSING_STRING, STR_INVALID, NOT_WHOLE, INS_NO_FOUND, 
             WRONG_NUM_PARAM, INVALID_PARAM, LINE_LENGTH, INVALID_LETTER, UNDEF_PARAM, UPPER_CASE, 
             LABEL_ENTRY_EXTERN, MISSING_LBL, MISSING_OPERAND, EXTRA_INPUT} errorList;



/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */

void printError(errorList, ...);

void *safeAlloc(alloc_t, ...);

#endif 