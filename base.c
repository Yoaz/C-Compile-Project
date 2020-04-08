                    /*** THIS IS THE BASE CODE, HERE WE WILL LOOP THROUGH USER/FILES 
                                           INPUT (1ST/2ND PASS)                 ***/

#include "base.h"



/* accpets *argv[] (file names) from cmd input and initiate assmebler */
void initiate(char *fileName){
    FILE *fp;
    
    strcat(fileName,".as"); /* add the file suffix */
    
    /* fetching files */
    if(!(fp = fopen(fileName, READ_ONLY))){
        printError(FAILED_OPEN, fileName); /* Will print FAILED_OPEN err */ 
        fclose(fp);
        return; /* name of file not found, move on to next file from argv[] if exist */
    }

    /* Debug */
    printf("Open file: \"%s\"\n", fileName); 

    /* Runs the first pass on the source file */
	if(firstRound(fp)) /* first round was successful */
	{
        /* updateLabelTable(fileName); /*TODO: build func to update symbol table after firstRound success */
        if(secondRound(fp)) /* second round was success */
        { 
            successFiles++; /* file went through the full parsing to meching code process with no errors, add to success files global counter */
            resetSpLine(pSpLine); /* free global splitted line var */
            fclose(fp);
            return; /* next file if exist */
        }
    }

    /* default: second or first round had issues */
    freeLblTable(); /*TODO: perhaps to merge one func freeAll to free all databases at end each file loop */
    fclose(fp);
    return; /* do not update symbol table nor go for second round on file */
}



/* first round through file content to examine the content */
boolean firstRound(FILE *fp)
{
    char *line, *tmpLine;
    IC=0, DC=0, numOfErrors = 0, numColumn = 0; /* reset global vars */
    
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
        
        /* splitLine with no errors -> if label exist */
        if(pSpLine -> lblFlag)
        {
            /* .entry/.extern label use is avoided - warning issued for this kind of use */
            if(strcmp(pSpLine -> cmd, DIR_ENTRY) && strcmp(pSpLine -> cmd, DIR_EXTERN)) 
                addLabel(); /* all the rest of commands, add to label table */
        }

        /* .string directive */
        if(!strcmp(pSpLine -> cmd, DIR_DATA))
        {

        }
        
        /* .data directive */
        else if(!strcmp(pSpLine -> cmd, DIR_DATA))
        {

        }

        /* instruction type of command */
        else
        {
            
        }

        
        free(line);  /* free alocated memory for line, to free room for next line if exist */
    }
    
    printLblTabel(); /* debug */

    /* if found errors in current file, first round failed, return false */
    if(numOfErrors)
        return false;

/* first round success */
return true;    
}


/* second round looping throug the file content to complete necesery parsing */
boolean secondRound(FILE *fp)
{

   IC = 0;
   rewind(fp); /* Setting FILE* pointer back to begining of file */
   
   return false;
}


/* reset all globals */
void resetGlobals()
{
    IC = 0;
    DC = 0;
    numColumn = 0;
    numRow = 0;
    numOfErrors = 0;
}