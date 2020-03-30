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
   
    /* check if line starts with label and store in label field in splittedLine global char if does,
    * otherwise prints relevant error if label is illegal or continue if line doesnt start with label */
    if(token && isLabel(token))  /* if not null and the first part is a label */
    {
        if(fetchLabel(token))
        {
            /* The next part should be the directive or operation */
            token = strtok(NULL, TOKEN_DELIM);
        }  

        /* label have errors, return false to fetch next line */
        else
            return false;
        
    }

    /* empty restOfLine -> missing directive or operation */
    if (!token) 
	{
		printError(MISSING_CMD);
		resetSpLine(pSpLine); /* reset splitted line fields */
        numOfErrors++;
		return false;
	}
 
     /* else, line isn't empty, check command as next token in order/first token in line must be one of the system commands */
     if(!isInstType(token) && !isDirType(token))
     {
        printError(UNDEF_CMD, token);
		resetSpLine(pSpLine); /* reset splitted line fields */
        numOfErrors++;
		return false;   
     }
    
    /* token is legit system command, store in global split line */
    pSpLine -> cmd = (char *)safeAlloc(sCalloc, strlen(token)+1, sizeof(char)); /* +1 for '\0' */
    strcpy(pSpLine -> cmd, token);
    printf("This is the cmd: %s, in size: %d \n", pSpLine -> cmd, strlen(pSpLine -> cmd)); /* debug */

    
    /*
     * each parseCmd func will recieve restOfLine content to examine
     * and parse accordingly, will print related errors and return true if no such errors  
     */
    if(!strcmp(token, DIR_DATA)) /* This is a .data directive, split the arguments by 'COMMA' (,) */
    {

        /*parseData();*/
    }

    else if(!strcmp(token, DIR_STRING)) /* This is a .string directive, split the arguments by 'COMMA' (,) */
    {
        /*parseString();*/
    }
    
    else if(!strcmp(token, DIR_ENTRY) || !strcmp(token, DIR_EXTERN))
    {

        if(!parseExternEntry(strtok(NULL, ""))) /* parse for extern/entry dir cmd's */
            resetSpLine(pSpLine); /* reset splitted line fields */
            return false;
    }

    else /* The other operations use the normal seperator */
    {
        
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
        if(p -> cmd)
            free(p -> cmd);
    }
    
    /*TODO: complete this reset */
}


/* computes rather token is of type instruction command */
boolean isInstType(char *token)
{
   /* all the saved words in our program */
   if(!(
   strcmp(token,"r0") && strcmp(token,"r1") && strcmp(token,"r2") && strcmp(token,"r3") && strcmp(token,"r4") &&
   strcmp(token,"r5") && strcmp(token,"r6") && strcmp(token,"r7") &&
   strcmp(token,"mov") && strcmp(token,"cmp") && strcmp(token,"add") && strcmp(token,"sub") &&
   strcmp(token,"lea") && strcmp(token,"clr") && strcmp(token,"not") && strcmp(token,"inc") &&
   strcmp(token,"dec") && strcmp(token,"jmp") && strcmp(token,"bne") && strcmp(token,"red") &&
   strcmp(token,"prn") && strcmp(token,"jsr") && strcmp(token,"rts") && strcmp(token,"stop")
   ))
	   return true;
	
	return false;
}


/* compute rather token is a directive command */
boolean isDirType(char *token)
{
   if(!(
   strcmp(token, ".data") && 
   strcmp(token, ".string") &&
   strcmp(token, ".entry") &&
   strcmp(token, ".extern")
   ))
        return true;

    return false;
}

/*                                                                                                           *\
********************************************* LABLE PARSING ***************************************************
\*                                                                                                           */


boolean fetchLabel(char *token)
{
    char *lbl; /* if token is label, will hold token (label) minus the ':' at end of token */

    lbl = (char *)safeAlloc(sCalloc, strlen(token) -1, sizeof(char)); /* minus the ':' in the token label */
    strncpy(lbl, token, strlen(token)-1); /* copy to local lbl */

    if(!findLabel(lbl) && isLegitLabel(lbl)) /* if label has legit syntax */ 
    { 
        pSpLine -> lblFlag = true;
        /* copy the label name to the global splitted line label field */
        pSpLine -> label = (char *)safeAlloc(sMalloc, strlen(lbl)); /* strlen(token) includes ':' at end */															
        strncpy(pSpLine -> label, lbl, strlen(lbl)); /* -1 to remove COLON ':' char from input label */	
        printf("This is the stored label: %s \n", pSpLine -> label);
        free(lbl); 
        return true;
    }
    
    free(lbl);											

    return false;
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
            for (i = 1; i < (strlen(label)); i++)
            {
                /* check if contain illegal label chars, print related error if does, return false */
                if(!isalnum(label[i]))
                {
                    printError(ILLEGAL_LABEL_CHARS, label);
                    numOfErrors++;
                    return false;
                }
            }

            /* check if label is a saved word (cannot be dir word as illegal char "." ), print related error if does, return false */
            if(isInstType(label))
            {
                printError(SAVED_WORD, label);
                numOfErrors++;
                return false;
            }
        }

        /* label starts with a number */
        else
        {
            printError(LABEL_NUMERIC_START, label);
            numOfErrors++;
            return false;
        }
    }

    /* length of label is above limit allowed */
    else
    {
        printError(LABEL_LENGTH, label);
        numOfErrors++;
        return false;
    }

    return true;
}



/*                                                                                                           *\
********************************************* DIRECTIVE PARSING ***********************************************
\*                                                                                                           */

/* will parse rest of line with cmd: .entry/.extern directives */
boolean parseExternEntry(char *restOfLine)
{
    char *token;

    if(!restOfLine) /* missing label */
    {
        numOfErrors++;
        printError(MISSING_LBL);
        return false;
    }

    token = strtok(restOfLine, TOKEN_DELIM);

    /* next part should be label */
    if(!isLegitLabel(token)) /* extry/entry label is illegal return false */
    {
        numOfErrors++;
        return false;
    }

    /* TODO: add the arguement to the splittedLine global var */

    /* no more input in line (only one legit label after .entry/.extern) is allowed */
    if(token = strtok(NULL, ""))
    {
        numOfErrors++;
        printError(EXTRA_INPUT, token);
        return false;
    }
    
    return true;
}


/*                                                                                                           *\
********************************************* INSTRUCTION PARSING *********************************************
\*                                                                                                           */

/* TODO: parseInstruction function */
void parseInstruction(){

}