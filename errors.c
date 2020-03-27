#include "errors.h"


/*                                                                                                           *\
********************************************* SYSTEM ERRORS ***************************************************
\*                                                                                                           */

/* 
* Generic Safe Memory Allocation: 
* if Malloc -> one more arg = size_t bytes required, besides must arg
* If Calloc -> two more arg = size_t length and size_t = times of length required, besides must arg
* if realloc -> two more arg = void *pointer and size_t = bytes required, besides must arg 
* Passing undefined alloc_t will generate compilation error
* Mis-Passing enough arguments based on alloc_t requirements will cause undefined behviour
*/
void *safeAlloc(alloc_t allocation_type, ...)
{
	void *temp; /* pointer to the new memory block */
    va_list ap;

    va_start(ap, allocation_type); 

    switch(allocation_type)
    {
        case(sMalloc):
             /* Malloc requires just 1 argc, size of bytes */
            temp = malloc(va_arg(ap, size_t));
            break;
        case(sCalloc):
            /* Calloc requires 2 args, length, size_t size */
            temp = calloc(va_arg(ap, size_t), va_arg(ap, size_t));
            break;
        case(sRealloc):
            /* Calloc requires 2 args, void *ptr, new size */
            temp = realloc(va_arg(ap, void*), va_arg(ap, size_t));
            break;
        default: /* Case of passing empty alloc_t variable */
            fprintf(stderr, "No Such Allocation Memory Type!");
            exit(1);
            break;

        va_end(ap);
    }
    /* check allocation failure */
    if(!temp){
        fprintf(stderr, "Allocation Failed! \n");
        exit(1);
    }

    return temp;
}



/*                                                                                                           *\
********************************************* INTERNAL ERRORS *************************************************
\*                                                                                                           */

/* The function accepts error title based on enum and decimal, string (OPTIONAL (depend on type of error)) and print the specific error (GLOBAL - row, col) */
void printError(errorList errorTitle, ...)
{
    va_list ap;
    
    va_start(ap, errorTitle);

    switch (errorTitle)
    {
        /* FILE */
        case NO_FILES: /* no arguments in command line except the program name */
            fprintf(stderr,"No files to compile\n");
            exit(1); /* case of fatal error, exit program */
            break;
        case FAILED_OPEN: /* the file not found */
            fprintf(stderr,"Failed to open \"%s\"\n",va_arg(ap, char *));
            break;
        case ERRORS_IN_FILE:
            fprintf(stderr, "Found %d errors in current file: %s\n", va_arg(ap, int), va_arg(ap, char*));
            break;
        case WRONG_FILES:
            fprintf(stderr, "None of the files provided seccuessfully translated\\found\n");
            break;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        case MISSING_DR_OP:
            fprintf(stderr, "Missing directive or instruction commands\n");
            break;

         /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        
        /* label */
        case LABEL_NUMERIC_START: /* label start with non aplphabetic */
            fprintf(stderr,"ERROR[%d,%d]:The label \"%s\" cannot start with non alphabetic letter\n",numRow,numColumn,va_arg(ap, char *));
            break;
        case LABEL_LENGTH: /* length of the label longer then 31 letters */
            fprintf(stderr,"ERROR[%d,%d]:The label \"%s\" is longer than 31 letters\n",numRow,numColumn,va_arg(ap, char *));
            break;
        case ILLEGAL_LABEL_CHARS: /* label contains illegal chars */
            fprintf(stderr,"ERROR[%d,%d]:The label \"%s\" contains illegal chars\n",numRow,numColumn,va_arg(ap, char *));
            break;
        case LABEL_DEFINED: /* redefinition of label */
            fprintf(stderr,"ERROR[%d,%d]:The label \"%s\" already defined\n",numRow,numColumn,va_arg(ap, char *));
            break;
        case SAVED_WORD: /* label is saved word */
            fprintf(stderr,"ERROR[%d,%d]:The label \"%s\" is a saved word in assembly\n",numRow,numColumn,va_arg(ap, char *));
            break;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

        /* Directive */   
        case DIR_NO_FOUND: /* directive not found */
            fprintf(stderr,"ERROR[%d,%d]:The directive \"%s\" is not found\n",numRow,numColumn,va_arg(ap, char *));
            break;
    
        /* .data */    
        case DATA_INVALID: /* invalid data */
            fprintf(stderr,"ERROR[%d,%d]: \"%s\" is invalid insertion with .data\n",numRow,numColumn,va_arg(ap, char *));
            break;

        /* .string */
        case STR_INVALID: /* invalid stirng */
            fprintf(stderr,"ERROR[%d,%d]: \"%s\" is invalid insertion with .string\n",numRow,numColumn,va_arg(ap, char *));
            break;

        case NOT_WHOLE: /* float number */
            fprintf(stderr,"ERROR[%d,%d]: \"%s\" is not whole number\n",numRow,numColumn,va_arg(ap, char *));
            break;

        case INS_NO_FOUND: /* instruction not found */
            fprintf(stderr,"ERROR[%d,%d]:The instruction \"%s\" is not found\n",numRow,numColumn,va_arg(ap, char *));
            break;
        
        case WRONG_NUM_PARAM: /* wrong number of parameters */
            fprintf(stderr,"ERROR[%d,%d]:Wrong number of param for  \"%s\"\n",numRow,numColumn,va_arg(ap, char *));
            break;
        
        case INVALID_PARAM: /* invalid parameters */
            fprintf(stderr,"ERROR[%d,%d]:Invalid parameters for \"%s\"\n",numRow,numColumn,va_arg(ap, char *));
            break;
        
        case LINE_LENGTH: /* to long line in file */
            fprintf(stderr,"ERROR[%d,0]:The line is over 80 letters\n",numRow,numColumn);
            break;

        case INVALID_LETTER: /* invalid letter */
            fprintf(stderr,"ERROR[%d,%d]: \"%s\" invalid letter\n",numRow,numColumn,va_arg(ap, char *));
            break;

        case UNDEF_PARAM: /* undefined  param */
            fprintf(stderr,"ERROR[%d,%d]: \"%s\" is undefined\n",numRow,numColumn,va_arg(ap, char *));
            break;

        case UPPER_CASE: /* not lower case */
            fprintf(stderr,"ERROR[%d,%d]: \"%s\" cant contain upper case letter\n",numRow,numColumn,va_arg(ap, char *));
            break;
    }
    va_end(ap);
}