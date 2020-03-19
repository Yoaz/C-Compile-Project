#include "errors.h"


/*                                                                                                           *\
********************************************* SYSTEM ERRORS ***************************************************
\*                                                                                                           */

/* 
* Generic Safe Memory Allocation: 
* if Malloc -> one more arg = size_t bytes required, besides must arg
* If Calloc -> two more arg = size_t length and size_t times of length required, besides must arg
* if realloc -> two more arg = void *pointer and size_t bytes required, besides must arg 
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
        case:(sCalloc):
            /* Calloc requires 2 args, length, size_t size */
            temp = calloc(va_arg(ap, size_t), va_arg(ap, size_t)) 
            break;
        case(sRealloc):
            /* Calloc requires 2 args, void *ptr, new size */
            temp = realloc(temp, va_arg(ap, size_t), va_arg(ap, size_t));
            break;
        default:
            /*TODO add to internal error */
            break;

        va_end(ap);

        /* check allocation failure */
        if(!temp){
            fprintf(stderr, "Allocation Failed! \n");
            exit(1);
        }
    }
}



/*                                                                                                           *\
********************************************* INTERNAL ERRORS *************************************************
\*                                                                                                           */


/*The function returns the error message from the error code given to it */
void getErrorMessage(StatusCode code,char* message)
{
    switch (code) {
    case success:
        break;
    case wrong_number_of_operands:
        strcpy(message,"Wrong number of operands");
        break;
    case unrecognized_command:
        strcpy(message,"Unrecognized command");
        break;
    case illigal_symbol_name:
        strcpy(message,"Illigal symbol name");
        break;
    case redeclaration_of_symbol:
        strcpy(message,"Symbol already exist");
        break;
    case symbol_doesnt_exist:
        strcpy(message,"Symbol doesn't exist");
        break;
    case data_syntax_error:
        strcpy(message,".data syntax error");
        break;
    case string_syntax_error:
        strcpy(message,".string syntax error");
        break;
    case struct_syntax_error:
        strcpy(message,".struct syntax error");
        break;
    case symbol_not_recognized:
        strcpy(message,"symbol not recognized error");
        break;
    case operand_type_error:
        strcpy(message,"operand type error");
        break;
    case row_count_error:
        strcpy(message,"counting summary of rows error");
        break;
    case inst_sum_error:
        strcpy(message,"instruction rows summary error");
        break;
    case invalid_source_dest_operands:
        strcpy(message,"Invalid source or destination operands");
        break;
    }
}

/*The function will print the error with the line the error appeared in, and the file name in which the error appeared in */
void printError(StatusCode code, int line, char* fileName)
{
    char errorMessage[MAX_FILE_LENGTH];
    if(code<0)
    {
        getErrorMessage(code,errorMessage);
        fprintf(stderr,"Error: %s. On line: %d and file: %s\n",errorMessage,line,fileName);
    }
}