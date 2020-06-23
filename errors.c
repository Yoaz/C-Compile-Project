#include "errors.h"
                    /*** THIS IS THE ERRORS SECTION, SYSTEM & INTERNAL  
                                     ERROR HANDLERS               ***/

/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                SYSTEM ERRORS 
***************************************************************************************************************
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
***************************************************************************************************************                                                                                                         
                                                INTERNAL ERRORS 
***************************************************************************************************************
\*                                                                                                           */

/* 
* Internal error exception function. accepts enum error title correspond to error type, 
* string (OPTIONAL (depend on type of error)) 
* and print the specific error (GLOBAL - row, col) 
*/
void printError(errorList errorTitle, ...)
{
    va_list ap;
    
    va_start(ap, errorTitle);

    switch (errorTitle)
    {
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FILE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        case NO_FILES: /* no arguments in command line except the program name */
            fprintf(stderr,"No files to compile\n");
            exit(1); /* case of fatal error, exit program */
            break;
        case FAILED_OPEN: /* the file not found */
            fprintf(stderr,"Failed to open \"%s\"\n",va_arg(ap, char *));
            break;
        case ERRORS_IN_FILE:
            fprintf(stderr, "\tFound %d errors in current file: %s.as\n", va_arg(ap, int), va_arg(ap, char*));
            break;
        case WRONG_FILES:
            fprintf(stderr, "None of the files provided seccuessfully translated\\found\n");
            break;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMAND ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        case MISSING_CMD:
            fprintf(stderr, "ERROR[Line -> %d]: Missing directive or instruction commands\n", numRow);
            break;
        case UNDEF_CMD:
            fprintf(stderr, "ERROR[Line -> %d]: Undefined directive or instruction command: %s\n", numRow,va_arg(ap, char *));
            break;

         /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LABEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/ 
        case LABEL_NUMERIC_START: /* label start with non aplphabetic */
            fprintf(stderr,"ERROR[Line -> %d]: The label \"%s\" cannot start with non alphabetic letter\n",numRow,va_arg(ap, char *));
            break;
        case LABEL_LENGTH: /* length of the label longer then 31 letters */
            fprintf(stderr,"ERROR[Line -> %d]: The label \"%s\" is longer than 31 letters\n",numRow,va_arg(ap, char *));
            break;
        case ILLEGAL_LABEL_CHARS: /* label contains illegal chars */
            fprintf(stderr,"ERROR[Line -> %d]: The label \"%s\" contains illegal chars\n",numRow,va_arg(ap, char *));
            break;
        case LABEL_EXIST: /* redefinition of label */
            fprintf(stderr,"ERROR[Line -> %d]: The label \"%s\" already defined\n",numRow,va_arg(ap, char *));
            break;
        case SAVED_WORD: /* label is saved word */
            fprintf(stderr,"ERROR[Line -> %d]: The label \"%s\" is a saved word in assembly\n",numRow,va_arg(ap, char *));
            break;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DIRECTIVE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* general*/
        case DIR_NO_FOUND: /* directive not found */
            fprintf(stderr,"ERROR[Line -> %d]: The directive \"%s\" is not found\n",numRow,va_arg(ap, char *));
            break;
    
        /* .data */ 
        case MISSING_DATA: /* missing data */
            fprintf(stderr,"ERROR[Line -> %d]: Missing data value with .data command\n",numRow);
            break;
        case DATA_INVALID: /* invalid data */
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" is invalid insertion with .data\n",numRow,va_arg(ap, char *));
            break;
        case NOT_WHOLE: /* float number */
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" is not whole number\n",numRow,va_arg(ap, char *));
            break;

        /* .string */
        case MISSING_STRING: /* missing string */
            fprintf(stderr,"ERROR[Line -> %d]: Missing string in .string command\n",numRow);
            break;
        case MISSING_STRING_START: /* missing string start decleration */
            fprintf(stderr,"ERROR[Line -> %d]: Missing quotation string start mark in .string command, for string: %s\n",numRow,va_arg(ap, char *));
            break;
        case MISSING_STRING_END: /* missing string end decleration */
            fprintf(stderr,"ERROR[Line -> %d]: Missing quotation string end mark in .string command, for string: %s\n",numRow,va_arg(ap, char *));
            break; 
        case ILLEGAL_BEFORE_STRING: /* illegal input before string decleration */
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" is invalid insertion with .string\n",numRow,va_arg(ap, char *));
            break;
        case STR_INVALID: /* invalid string */
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" is invalid insertion with .string\n",numRow,va_arg(ap, char *));
            break;       
        case LINE_LENGTH: /* to long line in file */
            fprintf(stderr,"ERROR[%d,0]: The line is over 80 letters\n",numRow);
            break;
        
        /* .extern  .entry */
        case LABEL_ENTRY_EXTERN: /* warning for use of label with .entry or .extern */
            fprintf(stderr,"--WARNING--: \"%s\" label use before .entry or .extern command\n",va_arg(ap, char *));
            break;
        case MISSING_LBL: /* missing label argument in .entry/.extern command */
            fprintf(stderr,"ERROR[Line -> %d]: Missing label\n",numRow);
            break; 
        case MISSING_LBL_DEF:
            fprintf(stderr,"ERROR[Line -> %d]: Label: \"%s\" defined as .entry, but, missing in file definition\n",numRow, va_arg(ap, char *));
            break;
        case EXTERNAL_LABEL: /* duplicate label declaration - already declared as external label */
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" Label already declared as external type of label\n",numRow,va_arg(ap, char *));
            break;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ INSTRUCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        /* general */
        case MISSING_ARG: /* missing operand for instruction command */
            fprintf(stderr,"ERROR[Line -> %d]: Missing operand\n",numRow);
            break;
        case TOO_MANY_ARGS: /* warning for use of label with .entry or .extern */
            fprintf(stderr,"ERROR[Line -> %d]: Too many args for this command\n",numRow);
            break;  
        case TOO_FEW_ARGS: /* warning for use of label with .entry or .extern */
            fprintf(stderr,"ERROR[Line -> %d]: Too few args for this command\n",numRow);
            break;  
        case INVALID_INST_ARG:
            fprintf(stderr,"ERROR[Line -> %d]: Invalid instruction command arg \"%s\"\n",numRow, va_arg(ap, char *));
            break; 
        case INVALID_REG_REF_SYNT:
            fprintf(stderr,"ERROR[Line -> %d]: Invalid reference to register syntax arg \"%s\"\n",numRow, va_arg(ap, char *));
            break;     
        case IMMIDIET_SYNTAX:
            fprintf(stderr,"ERROR[Line -> %d]: Wrong immidiet arg type \"%s\"\n",numRow, va_arg(ap, char *));
            break;
        case NUM_RANGE:
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" is off range for this command\n",numRow, va_arg(ap, char *));
            break; 
        case MISSING_LBL_AS_ARG:
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" label is passed as instruction argument, but, not defined (as .entry or as .extern)\n",numRow, va_arg(ap, char *));
            break;

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GENERAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        case EXTRA_INPUT:
            fprintf(stderr,"ERROR[Line -> %d]: \"%s\" is an extraneous input in line\n",numRow, va_arg(ap, char *));
            break;
        case MISSING_ARG_SEP:
            fprintf(stderr,"ERROR[Line -> %d]: Missing argument comma (,) separator\n",numRow);
            break;
        case ILLEGAL_ARG_SEP:
            fprintf(stderr,"ERROR[Line -> %d]: Illegal argument comma (,) separator\n",numRow);
            break;

    }
    va_end(ap);
}