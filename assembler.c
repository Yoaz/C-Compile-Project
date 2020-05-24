/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
AUTHOR: Yoaz Shmider
ASSIGNMENT: Assembler -> Rules Apply
VERSION: 1.0
***************************************************************************************************************
\*                                                                                                           */

#include "assembler.h"


int main(int argc, char *argv[])
{   
    successFiles = 0; /* reset global success files var one time only every program run session */
   
    /* In case file names werent provided via command run */
    if(argc <= 1)
        printError(NO_FILES); /* Will print NO_FILES err and exit program since it is a fatal error */
    
    /* loop through main argv's after name of program */
    while(--argc)
    {   
        resetGlobals(); /* for every new file round, reset all globals */
        initiate(*(++argv));
        if(numOfErrors)
            printError(ERRORS_IN_FILE, numOfErrors, *argv);
    }

    /* if successFiles (files provided via cmd line who went through the whole proccess with no errors and 
    * translated to mechine code seccefully) is 0, print error "no files provided found" */
    if(!successFiles)
        printError(WRONG_FILES);

    return 0;
}
    






