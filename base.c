                    /*** THIS IS THE BASE CODE, HERE WE WILL LOOP THROUGH USER/FILES 
                                           INPUT (1ST/2ND PASS)                 ***/

#include "base.h"



/* first round through file content to examine the content */
int firstRound(FILE *fp)
{
    char *line, *tmpLine, *token;
    IC=0, DC=0, numErrors = 0, numColumn = 0; /* reset global vars */
    int c;
    boolean lblFlag = false;
    
    /* As long as not end of file keep fetch lines from file */
    while(!feof(fp))
    {
        fetchLine(fp, &line);

        if(!relevantToParse(line)) /* passing current line to check if relevant to parse (maybe empty, comment etc) */
            continue;

        /* time to parse which type of line this is */
        strcpy(tmpLine, line); /* To preserve original line raw */

        /* start reading first token from line with space delim, as we are fetching raw line from file using 
        * fetchLine(), but, while 'avoiding unecesery legal input extra white tabs' and make sure only one space is
        * preserved between every two parts of string that were originally typed with one or more white tabs */
        token = strtok(tmpLine, SPACE); 

        while(*tmpLine != STRING_END)
        {
            c = *(tmpLine++); /* assing c with each line char */ 
            numColumn++; /* add +1 to global column counter
            /* already made sure the line isn't empty nor comment ';' nor undefined via relevantToCheck(char *) */
            if(c == COLON) /* since we check first token, must be label or undefined */
            {
                
                token = strtok(line, COLON); /* cuts the label from the string */
                fetchLable(token); /* checks if the label is correct */
                lblFlag = true;

            }
            if(c == DOT) /* instruction */
            {
                if(lblFlag)
                {
                    parseInstruction(line+ind);
                    lblFlag = 0;
                }
                else
                    parseDirective(line+ind);
                return;
            }
        }  
        free(line);  /* free alocated memory for line */
    }

return 0;    
}

/* second round looping throug the file content to complete necesery parsing */
void secondPass(char *fileName, FILE *fp)
{

   IC = 0;
   rewind(fp); /* Setting FILE* pointer back to begining of file */

   




   
   
}