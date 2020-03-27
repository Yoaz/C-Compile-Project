                    /*** IN THIS FILE WE WILL HAVE ALL THE FUNCTION RELATED TO PARSING TXT/STRING
                                             EXAMINE LINES CONTENT ETC                       ***/


#include "parse.h"


 
/*                                                                                                           *\
********************************************* GENERAL PARSING *************************************************
\*                                                                                                           */




/* The function checks wether a given char is a white char (exclude '\n') */ 
boolean whiteCh(char ch)
{
    if(ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r')
        return true;
    return false;
}


/* checks if there is a needs to parse a given line */
boolean relevantToParse(char *line)
{
	char *p; 

	if (!line || *line == COMMENT_SIGN || *line == LINE_BREAK || !strlen(line)){ /* ignore undefined/comments/empty lines */
        printf("IGNORED LINE: %s\n", line); /* debug */
        return false;
    }

	for (p = line; p<p + strlen(line); p++)
    {
		if (!isspace(*p)) /* The line isn't empty */
        {
            printf("DIDN'T IGNORE LINE: %s\n", line); /* debug */
			return true;     
        } 
    }
    
	return false;
}

/* will split the line and assign to relevant global splitted line var */
boolean splitLine(char *line)
{
    char *token;
    int c;
    pSpLine = (spLine *)safeAlloc(sCalloc, 1, sizeof(spLine)); /* assign memory to global splitted line var */

    printf("REACHED SPLIT LINE: %s\n", line); /* debug */

    /* start reading first token from line with space delim, as we are fetching raw line from file using 
    * fetchLine(), but, while 'avoiding unecesery legal input extra white tabs' and make sure only one space is
    * preserved between every two parts of string that were originally typed with one or more white tabs */
    token = strtok(line, TOKEN_DELIM); 
    printf("This is the 1st token: %s, in size: %d \n", token, strlen(token)); /* debug */
   
    if (token && isLabel(token))  /* if not null and the first part is a label */
	{
        if(isLegitLabel(token)) /* if label has legit syntax */ 
        { 
            pSpLine -> lblFlag = true;
            /* copy the label name to the global splitted line label field */
            pSpLine -> label = (char *)safeAlloc(sMalloc, strlen(token)); /* strlen(token) includes ':' at end */															
            strncpy(pSpLine -> label, token, strlen(token)-1); /* -1 to remove COLON ':' char from input label */	
            printf("This is the stored label: %s \n", pSpLine -> label);
        }	
											
	}

    /* The next part should be the directive or operation */
    token = strtok(NULL, TOKEN_DELIM);

    if (!token) /* missing directive or operation */
	{
		printError(MISSING_DR_OP);
		resetSpLine(pSpLine); /* reset splitted line fields */
        numOfErrors++;
		return false;
	}




    return true;
}

/* reset splitted line global var */
void resetSpLine(spLine *p)
{
    if(p)
	{
        p -> lblFlag = false;
        if(p -> label)
            free(p -> label);
    }
    
    /*TODO: complete this reset */
}

/*                                                                                                           *\
********************************************* LABLE PARSING ***************************************************
\*                                                                                                           */


void fetchLable(char *token)
{
    /*TODO*/
}


/* Checks if a token is a label definition. */
boolean isLabel(char *token)
{   
    if(token[strlen(token)-1] == COLON){
        printf("This is a label: %s\n", token);
        return true;
    }
	
    return false;
}


/* 
* Checks whether the given label is a valid:
* - not a system saved world
* - not surpass max label length limit
* - doesnt start with numeric
* - doesnt contain illegal chars (only letters and decimal numbers allowed) 
*/
boolean isLegitLabel(char *label) 
{
    unsigned int i;
    
    if (strlen(label) <= MAX_LABEL_SIZE) 
    {
        if(isalpha(*label))
        {
            for (i = 1; i < (strlen(label)-1); i++)
            {
                /* check if contain illegal label chars, print related error if does, return false */
                if(!isalnum(label[i]))
                {
                    printError(ILLEGAL_LABEL_CHARS, pSpLine -> label);
                    numOfErrors++;
                    return false;
                }
            }

            /* check if label is a saved word, print related error if does, return false */
            if(isSavedWord(label))
            {
                printError(SAVED_WORD, pSpLine -> label);
                numOfErrors++;
                return false;
            }
        }

        /* label starts with a number */
        else
        {
            printError(LABEL_NUMERIC_START, pSpLine -> label);
            numOfErrors++;
            return false;
        }
    }

    /* length of label is above limit allowed */
    else
    {
        printError(LABEL_LENGTH, pSpLine -> label);
        numOfErrors++;
        return false;
    }

    return true;
}


/* this function checks if the label is named as saved word in assembly */
boolean isSavedWord(char *label)
{
   /* all the saved words in our program */
   if(!(
   strcmp(label,"r0:") && strcmp(label,"r1:") && strcmp(label,"r2:") && strcmp(label,"r3:") && strcmp(label,"r4:") &&
   strcmp(label,"r5:") && strcmp(label,"r6:") && strcmp(label,"r7:") &&
   strcmp(label,"mov:") && strcmp(label,"cmp:") && strcmp(label,"add:") && strcmp(label,"sub:") &&
   strcmp(label,"lea:") && strcmp(label,"clr:") && strcmp(label,"not:") && strcmp(label,"inc:") &&
   strcmp(label,"dec:") && strcmp(label,"jmp:") && strcmp(label,"bne:") && strcmp(label,"red:") &&
   strcmp(label,"prn:") && strcmp(label,"jsr:") && strcmp(label,"rts:") && strcmp(label,"stop:") &&
   strcmp(label,".string:") && strcmp(label,".data:") && strcmp(label,".extern:") && strcmp(label,".entry:")
   ))
	   return true;
	
	return false;
}

/* this function is return the type of the current instruction */
int typeCheck(char* type)
{
   if(!strcmp(type, ".data"))
      return 1;
   if(!strcmp(type, ".string"))
      return 2;
   if(!strcmp(type, ".entry"))
      return 3;
   if(!strcmp(type, ".extern"))
      return 4;
   return 0;
}

/*                                                                                                           *\
********************************************* DIRECTIVE PARSING ***********************************************
\*                                                                                                           */

/* TODO: parseDirective function */
void parseDirective(){

}


/*                                                                                                           *\
********************************************* INSTRUCTION PARSING *********************************************
\*                                                                                                           */

/* TODO: parseInstruction function */
void parseInstruction(){

}