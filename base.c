                    /*** THIS IS THE BASE CODE, HERE WE WILL LOOP THROUGH USER/FILES 
                                           INPUT (1ST/2ND PASS)                 ***/

#include "base.h"



/* accpets *argv[] (file names) from cmd input and initiate assmebler */
void initiate(char *fileName)
{
    fileObject *fileOb;
    
    /* fetching files */
    if(!(fileOb=fetchFile(fileName, READ_ONLY)))
        return;

    /* Debug */
    printf("Open file: \"%s.as\"\n", fileOb -> rawName); 

    /* Runs the first pass on the source file */
	if(firstRound(fileOb -> src)) /* first round was successful */
	{
        updateLblTable(); /* update label table with starting address (100) and .data/.string with sum IC */
        printLblTabel(); /* debug */

        if(secondRound(fileOb)) /* second round was success */
        { 
            successFiles++; /* file went through the full parsing to meching code process with no errors, add to success files global counter */
            printLblTabel(); /* debug */
            closeFile(fileOb);
            return; /* next file if exist */
        }
    }

    freeDtList();
    /* default: second or first round had issues */
    printLblTabel(); /* debug */
     /*TODO: perhaps to merge one func freeAll to free all databases at end each file loop */
    closeFile(fileOb);
    return; /* do not update symbol table nor go for second round on file */
}



/* first round through file content to examine the content, built symbol table */
boolean firstRound(FILE *fp)
{
    char *line;
    IC=0, DC=0, numOfErrors = 0, numColumn = 0; /* reset global vars */
    argNode *a; /* to run on args */
    dtWord *curDW; /* to write data word node for .data, .string legit commands */
    char *tmp; /* to run on .string string argument chars */
    
    /* As long as not end of file keep fetch lines from file */
    while(1)
    {
        if(feof(fp))
            break; /* since file eof activated after trying to act on file once arrived end of file */
        
        /* fetch line from file and save to local line var */
        fetchLine(fp, &line);

        numRow++; /* global counter for current row num in current file */
        
        /* Debug */
        printf("\n%s-> size of line: %d\n", line, strlen(line));
        
        if(!relevantToParse(line)){ /* if line not relevant to parse (maybe undefined, empty, comment etc), skip */
            free(line);
            continue;
        }
        
        /* time to parse which type of line this is */
        if(!splitLine(line)) /* compute which RELEVANT line this is and split accordingly and assign to global splited line object */
        {   
            resetSpLine(pSpLine); /* free global splitted line var */
            free(line);    
            continue; /* if error occur during splitLine() then errors in line */
        }

        if(pSpLine -> lblFlag && findLabel(pSpLine -> label)) /* if label as first-token-in-line exist but already previously defined */
        {
            numOfErrors++;
            printError(LABEL_EXIST ,pSpLine -> label);
            resetSpLine(pSpLine);
            free(line); 
            continue;   
        }

        /* .extern */
        if(!strcmp(pSpLine -> cmd, DIR_EXTERN))
        {   
            /* in this case we add label as operand from .extern directive, reason why we know argsHead exist */
            addLabel(pSpLine -> argsHead -> name, L_EXTERNAL, 0);     
        }


        /* .entry */
        else if(!strcmp(pSpLine -> cmd, DIR_ENTRY))
        {   
            resetSpLine(pSpLine);
            free(line);
            continue;
        }

        /* .string */
        else if(!strcmp(pSpLine -> cmd, DIR_DATA))
        {
            if(pSpLine -> lblFlag)
            {
                addLabel(pSpLine -> label, L_DATA, DC);
            }
            
            /* build data word and add to data word list */
            /* we know argsHead exist as otherwise .data without arguments would not pass splitLine parsing as illegal */
            for(a = pSpLine -> argsHead; a; a = a -> next) /* run on args */
            {
                curDW = setDataWord(atoi(a -> name)); /* set new data word */
                addDtWordToDtList(&curDW);  /* add to data word to data word list */
                printWord(curDW); /* debug */
                printf("The word in decimal: %d\n", binCharArrToDec(curDW)); /* debug */
                curDW = NULL; 
            }

            increaseDC(); /* will increase data count depend on type of directive command */
        }
        
        /* .data */
        else if(!strcmp(pSpLine -> cmd, DIR_STRING))
        {
            if(pSpLine -> lblFlag)
            {
                addLabel(pSpLine -> label, L_STRING, DC);
            }

            /* build data word and add to data word list */
            /* we know argsHead exist as otherwise .string without arguments would not pass splitLine parsing as illegal */
            for(tmp = (pSpLine -> argsHead ->name) + 1; tmp < (pSpLine -> argsHead ->name) + (strlen(pSpLine -> argsHead ->name) - 1); tmp++) /* run on string chars minus "" marks */
            {
                curDW = setDataWord(*tmp); /* set new data word */
                addDtWordToDtList(&curDW);  /* add to data word to data word list */
                printWord(curDW); /* debug */
                printf("The word in char: %c\n", binCharArrToDec(curDW)); /* debug */
            }
            /* set end of string '/0' data word node to data word list */
            curDW = setDataWord(STRING_END); /* set new data word */
            addDtWordToDtList(&curDW);  /* add to data word to data word list */
            printWord(curDW); /* debug */
            printf("The word in char: %c\n", binCharArrToDec(curDW)); /* debug */

            curDW = NULL; 

            increaseDC(); /* will increase data count depend on type of directive command */
        }

        /* instruction */
        else
        {
            if(pSpLine -> lblFlag)
            {
                addLabel(pSpLine -> label, L_INST, IC);
            }

           increaseIC(); /* will increase data count depend on type of instruction command */ 
        }

        resetSpLine(pSpLine); /* free global splitted line var */
        free(line);  /* free allocated memory for line, to free room for next line if exist */
    }

    /* if found errors in current file, first round failed, return false */
    if(numOfErrors)
        return false;

/* first round success */
return true;    
}


/* second round looping throug the file content to complete necesery parsing */
boolean secondRound(fileObject *fileOb)
{
    int i = 0, sumIC = IC; /* sumIC - before IC reset for use in OBJECT file first creation, DC we have, not reseting */  
    IC = 0;
    rewind(fileOb -> src); /* Setting FILE* pointer back to begining of file */
    labelNode *p; 
    argNode *a; /* run on args tmp */
    char *line;

    /* As long as not end of file keep fetch lines from file */
    while(1)
    {
        if(feof(fileOb -> src))
            break; /* since file eof activated after trying to act on file once arrived end of file */

        /* fetch line from file and save to local line var */
        fetchLine(fileOb -> src, &line);

        numRow++; /* global counter for current row num in current file */
        
        /* Debug */
        printf("\n%s-> size of line: %d\n", line, strlen(line));
        
        if(!relevantToParse(line)) /* if line not relevant to parse (maybe undefined, empty, comment etc), skip */
        {
            free(line);
            continue;
        }
        
        /* 
        * time to parse which type of line this is, NO NEED to check the legitamacy of each line again, as we did in 
        * first round the parse part since secondRound() is called only if all lines are clean of syntax issues
        */
        splitLine(line); 

        /* .entry */
        if(!strcmp(pSpLine -> cmd, DIR_ENTRY))
        {
            
            /* look for the label in label table */
            p = findLabel(pSpLine -> argsHead -> name);

            if(!p) /* label doesnt exist in label tabel, but, defined in .entry directive */
            {
                numOfErrors++;
                printError(MISSING_LBL_DEF, pSpLine -> argsHead -> name);
            }

            else if(p) /* label exist */
            {    
                if(p -> type == L_EXTERNAL)  /* but, is declared as external as well */
                {
                    numOfErrors++;
                    printError(EXTERNAL_LABEL, p -> name);
                } 
                
                else /* label exist and not .external defined */
                {
                    /* TODO: complete file struct to hold all file * type and basic fchar *filaname */
                    writeEntry(fileOb, p -> name, p -> value); /* write to entry file */
                }
            
            }

        }

        /* .extern, .data, .string */
        else if(!strcmp(pSpLine -> cmd, DIR_EXTERN) || !strcmp(pSpLine -> cmd, DIR_DATA) || !strcmp(pSpLine -> cmd, DIR_STRING))
        {
            /* already taken care in first file pass */
            resetSpLine(pSpLine);
            free(line);
            continue;

        }

        /* instruction */
        else
        {
            if(pSpLine -> argsHead) /* instruction command with arguments */
            {
                a = pSpLine -> argsHead; /* to run on arguments */

                while(a) /* run on args */
                {
                    i++; /* using i counter for arg address order count */

                    if(whichAddArgType(a -> name) == DIRECT) /* label as argument */
                    {
                        p = findLabel(a -> name); /* look for the arg label in label table */

                        if(p) /* label exist in label table */
                        {
                            if(p -> type == L_EXTERNAL) /* label as arg, exist in label table and type external */
                            {
                                writeExtern(fileOb, p -> name, IC + STARTING_ADDRS + i); /* write to external file */
                            }
                        }   
                        else if(!p) /* label as argument, but, not declared as .extern or .entry, error */
                        {
                            numOfErrors++;
                            printError(MISSING_LBL_AS_ARG, a -> name);
                        } 
                                      
                    }
                    a = a -> next;
                }

                writeObject(fileOb, 0, 0, sumIC);
            }

            increaseIC(); /* will increase data count depend on type of instruction command */ 
            i = 0;
        }
    }

    /* if found errors in current file, second round failed, return false */
    if(numOfErrors)
        return false;

/* second round success */
return true;    
}



/* reset all globals */
void resetGlobals()
{
    freeLblTable();
    IC = 0;
    DC = 0;
    numColumn = 0;
    numRow = 0;
    numOfErrors = 0;
}