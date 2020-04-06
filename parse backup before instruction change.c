                    /*** IN THIS FILE WE WILL HAVE ALL THE FUNCTION RELATED TO PARSING TXT/STRING
                                             EXAMINE LINES CONTENT ETC                       ***/


#include "parse.h"


 
/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                 GENERAL PARSING 
***************************************************************************************************************
\*                                                                                                           */


/* 
* Core func in parsing the current input line, func parse each provided token from input 
* and compute rather legit based on assmebly rules, will drop related error if not and return false,
* otherwise, will split the line and assign to relevant global splitted line var 
*/
boolean splitLine(char *line)
{
    char *token;
    int c;
    argNode *arg; /* for use - run on args */

    pSpLine = (spLine *)safeAlloc(sCalloc, 1, sizeof(spLine)); /* assign memory to global splitted line var */

    printf("REACHED SPLIT LINE: %s\n", line); /* debug */

    /* start reading first token from line with space delim, as we are fetching raw line from file using 
    * fetchLine(), but, while 'avoiding unecesery legal input extra white tabs' and make sure only one space is
    * preserved between every two parts of string that were originally typed with one or more white tabs */
    token = strtok(line, TOKEN_DELIM); 
    printf("This is the 1st token: %s, in size: %d \n", token, strlen(token)); /* debug */
   
    /* check if line starts with label and store in label field in splittedLine global var if does,
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
        numOfErrors++;
		return false;
	}
 
     /* else, line isn't empty, check command as next token in order/first token in line must be one of the system commands */
     if(!isInstType(token) && !isDirType(token))
     {
        printError(UNDEF_CMD, token);
        numOfErrors++;
		return false;   
     }
    
    /* token is legit system command, store in global split line */
    pSpLine -> cmd = (char *)safeAlloc(sCalloc, strlen(token)+1, sizeof(char)); /* +1 for '\0' */
    strcpy(pSpLine -> cmd, token);
    printf("This is the cmd: %s, in size: %d \n", pSpLine -> cmd, strlen(pSpLine -> cmd)); /* debug */

    
    /*
     * each parseCmd() func will recieve restOfLine content to examine
     * and parse accordingly, will print related errors if there are issues and return false,
     * otherwise, store all related data in splitLine global var
     * and return true
     */
    if(!strcmp(token, DIR_STRING)) /* this is a .string directive */
    {
        if(!parseString(strtok(NULL, ""))) /* parse for .string dir cmd, if errors ocurred return false */
            return false;
        
    }
    
    else if(!strcmp(token, DIR_ENTRY) || !strcmp(token, DIR_EXTERN)) /* this is a .extern/.entry directive  */
    {
        if(!parseExternEntry(strtok(NULL, ""))) /* parse for extern/entry dir cmd's ocurred return false */
            return false;
    }

    else /* other operations use the normal seperator */
    {
        if(!parseCmd(strtok(NULL, ""))) /* parse for .data and all instructions cmd's */
            return false;

        /* TODO: check num of args for each command */
        if(!pSpLine -> argsHead) /* for instructions with 0 operands requirement  ('stop' in ex.) */
            return true; 


        /* arguments legit check for .data directive or instruction commands that requires at least 1 operand accordingly */
        for (arg = pSpLine -> argsHead; arg; arg = arg -> next)  
        {
            if(!strcmp(pSpLine -> cmd, DIR_DATA))
            {
                /* check arg is a number */
                if(!isNumber(arg -> name))
                {
                    printError(DATA_INVALID, arg -> name);
                    return false;
                }
                /* check num range valid for 15bits size */
                if(!numInRange(arg -> name))
                {
                    printError(NUM_RANGE, arg -> name);
                    return false;
                }
            }

            /* else, instruction command, check legit current IC line provided args */
            if(!legitInstArg(arg -> name))
            {
                return false;
            }   
            
        }  
    }

    if(pSpLine -> argsHead)
        printArgTabel();

    return true;
}


/* checks if there is a need to parse a given line */
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


/* The function checks wether a given char is of white char (exclude '\n') */ 
boolean whiteCh(char ch)
{
    if(ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r')
        return true;
    return false;
}


/* free splitted line global var */
void resetSpLine(spLine *p)
{
    argNode *a; /* for args list reset */

    if(p)
	{
        p -> lblFlag = false;
        if(p -> label)
            free(p -> label);
        if(p -> cmd)
            free(p -> cmd);
        if(p -> argsHead)
        {
            for (a = p -> argsHead; a -> next; a = a -> next) 
            {
                p = a -> next;
                free(a);
                a = p;
            }

            p ->  numArgs = 0;
        }
        free(p);
    }
    
    /*TODO: complete this reset */
}


/* computes rather token is of type instruction command */
boolean isInstType(char *token)
{
   /* all the saved words in our program */
   if(!(
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


/* add a single arg to an args list, if no list, head of current splitted line is assigned
* to point to current new arg */
boolean addArgToArgList(char *arg)
{
	argNode *p, *tmp; 

    if(!arg) /* safety major */
        return false;

    if(!pSpLine) /* safety major */
        return false;

    p = (argNode *)safeAlloc(sCalloc, 1, sizeof(argNode)); /* allocate for argNode */
    p -> name = safeAlloc(sCalloc, 1, strlen(arg) +1); /* allocate for arg name +1 for null terminater in string */
    strcpy(p -> name, arg); /* copy to arg name */

	if(!(pSpLine -> argsHead)) /* There is no head, the new argument is the head */
    {
        pSpLine -> argsHead = p;
		return true;
    }

	for (tmp = pSpLine -> argsHead; tmp -> next; tmp = tmp -> next) 
		; /* run till last node in current line args list */

	tmp -> next = p; /* add new node to list at the end */

	return true;
}

/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                 LABEL PARSING 
***************************************************************************************************************
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
***************************************************************************************************************                                                                                                         
                                                DIRECTIVE PARSING 
***************************************************************************************************************
\*                                                                                                           */


/* will parse rest of line with cmd: .entry/.extern directives */
boolean parseExternEntry(char *restOfLine)
{
    char *token;

    if(pSpLine -> lblFlag == true) /* use of label with .extern/.entry -> issue a warning (not an error) */
        printError(LABEL_ENTRY_EXTERN, pSpLine -> label);

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

    /* add the arguement to the splittedLine global var */
    if(!addArgToArgList(token))
    {
        return false;
    }

    /* no more input in line (only one legit label after .entry/.extern) is allowed */
    if(token = strtok(NULL, ""))
    {
        numOfErrors++;
        printError(EXTRA_INPUT, token);
        return false;
    }

    pSpLine ->  numArgs = 1; 


    return true;
}


/* parse .string arguments */
boolean parseString(char *restOfLine)
{
    char *temp, *firstQuot, *secondQuot;
    boolean inString = false;


    if(!restOfLine) /* missing string */
    {
        numOfErrors++;
        printError(MISSING_STRING);
        return false;
    }

    temp = restOfLine;

    if(!strchr(temp, STRING_START)) /* no quotation mark in rest of line after .string command */
    {
        printError(MISSING_STRING_START, temp);
        return false;
    }
    
    if(strchr(temp, STRING_START) == strrchr(temp, STRING_START)) /* no quotation mark to mark string end in .string command */
    {
        printError(MISSING_STRING_END, temp);
        return false;
    }

    /* line isn't empty and there's at least 2 different quotation marks in string after .string command */
    firstQuot = strchr(temp, STRING_START); /* assign address of first quotataion mark show */
    secondQuot = strrchr(temp, STRING_START); /* assign address of last quotataion mark show (already made sure they are different and existant */

    while(*temp) /* loop till string end */
    {
        if(whiteCh(*temp) && !inString) /* can be white space before begining of string declaration */
            continue;
        
        if((!whiteCh(*temp)) && (temp < firstQuot) && (!inString)) /* characters after .string command, but, before string declaration */
        {
            /* TODO: send the illegal input by using firstQuot address index and pointer begining index temp */
            printError(ILLEGAL_BEFORE_STRING);
            return false;
        }
        
        if(temp == firstQuot)
        {
            inString = true;

        }
        temp++;
    }

    /* add the arguement to the splittedLine global var */
    if(!addArgToArgList(restOfLine))
    {
        return false;
    }

    pSpLine ->  numArgs = 1;
    
    return true;
}


/* parse .data & instruction command arguments ISSUE: when .data odd num of arguments + error (illegal comma, not a number argument -> segmentation error) */
boolean parseCmd(char *restOfLine)
{
    int argc=0, argLen=0;
    char *temp;

 	enum
	{
		waitForArg,		   /* Waiting for arg. */
		readArg,		   /* Right now reads an arg  */
		waitForSep		   /* Wait for the 'specific' separator */
	} status = waitForArg; /* An status that will help with the parse */

	int i,		/* loop helper */
		startI; /* The beginning of the current arg */

    if(!restOfLine) /* no args data */
    {
        
        if(!strcmp(pSpLine -> cmd, "stop") || !strcmp(pSpLine ->  cmd, "rts")) /* no args for these commands is correct */
            return true;
        
        numOfErrors++; /* else, it is an error */
        
        if(!strcmp(pSpLine -> cmd, DIR_DATA))
            printError(MISSING_DATA);
        else    
            printError(MISSING_ARG);
        return false;
    }

	for (i = 0; restOfLine[i]; i++) /* Run on the line, char by char */
	{
		if (status == waitForArg) /* Waiting for the start of an arg. */
		{
			if (isspace(restOfLine[i])) /* Before arg can be white-spaces */
				continue;

			if (restOfLine[i] == COMMA) /* There is no arg */
			{
				printf("Illegal SEPARATOR. \n");
				return false;
			}
			else /* The start of the arg */
			{
				startI = i;
				status = readArg;
			}
		}
		else if (status == readArg) /* Read an arg. */
		{
			if (isspace(restOfLine[i]) || restOfLine[i] == COMMA) /* Space / Separator marks the end of the arg */
			{
				/* copy the arg to an allocated "string" */
				argLen = i - startI;
                temp = (char *)	safeAlloc(sCalloc,(argLen + 1), sizeof(char));																  /* i is the place in the line where the arg ends. */
				temp = strncpy(temp, restOfLine + startI, argLen); /* +1 for the null-terminator */
				temp[argLen] = '\0';																  /* Adds the null terminator to the end of the line. */
                if(!addArgToArgList(temp))
                {
                    free(temp);
                    return false;
                }

                argLen = 0;
				++argc;
                free(temp);

				status = waitForSep;
				if (restOfLine[i] == COMMA) /* There is separator, now need another arg. */
					status = waitForArg;
			}
			else /* Still reads the line */
				continue;
		}
		else if (status == waitForSep) /* Waiting for separator */
		{
			if (isspace(restOfLine[i])) /* Before separator can be white-spaces */
				continue;
			if (restOfLine[i] == COMMA) /* Find the separator, now need another arg */
				status = waitForArg;
			else /* There is an arg before separator! */
			{
				printf("Missing SEPARATOR! \n");
				return false;
			}
		}
	}

	if (status == waitForArg) /* There is a separator at the end of the line */
	{
		printf("Illegal SEPARATOR. \n");
        
		return false;
	}

	else if (status == readArg) /* There is arg at the end of the line */
    {
        /* copy the arg to an allocated "string" */
        argLen = i - startI;
        temp = (char *)	safeAlloc(sCalloc, (argLen + 1), sizeof(char));	/* i is the place in the line where the arg ends. */
        temp = strncpy(temp, restOfLine + startI, argLen); /* +1 for the null-terminator */
        temp[argLen] = '\0';	/* Adds the null terminator to the end of the line. */
        if(!addArgToArgList(temp))
        {
            free(temp);
            return false;
        }

        argLen = 0;
        ++argc;
        free(temp);
    }

	else if (argc == 0) /* line wasnt empty, but had white chars and no args  */
    {
        if(!strcmp(pSpLine -> cmd, "stop") || !strcmp(pSpLine ->  cmd, "rts"))
            return true;
        
        numOfErrors++;
        if(!strcmp(pSpLine -> cmd, DIR_DATA))
            printError(MISSING_DATA);
        else    
            printError(MISSING_ARG);
        return false;
    }

    pSpLine -> numArgs = argc; /* assign num of args to global splitted var */

	return true;
}


/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                            INSTRUCTION PARSING 
***************************************************************************************************************
\*                                                                                                           */


/* compute rather arg is legit instrucion command arg */
boolean legitInstArgs(char *arg)
{
    char *p;

    if(!arg)
        return false;

    if(strlen(arg) < 2) /* args must be 2+ in size */
    {
        numOfErrors++;
        printError(INVALID_INST_ARG, p);
        return false;
    }

    p = arg;

	if( (*p == ARG_REG) && (*p == ARG_IMMIDIET) && (*p == ARG_REF) ) /* args that starts with one of the following: '#, r, *' */
    {
        switch(*p)
        {
            case ARG_REG:
                if(!isdigit(*(++p)) || strlen(p) > 2)
                {
                    numOfErrors++;
                    printError(INVALID_REG_SYNT, p);
                    return false;
                }
                if(atoi(*(++p)) > 7 || atoi(*(++p)) < 0)
                {
                    numOfErrors++;
                    printError(INVALID_REG_RANGE, p);
                    return false;   
                }
                break;
            case ARG_IMMIDIET:
                if(!isNumber(*(++p))) /* after '#' must come valid number */
                {
                    numOfErrors++;
                    printError(IMMIDIET_SYNTAX, p);
                    return false;
                }
                break;
            case ARG_REF:
                if(!isReg(p))
                {
                    numOfErrors++;
                    printError(INVALID_REG_REF_SYNT, p);
                    return false;
                }

        }

        if(!isLegitLabel(arg)) /* label as an arg */
            return false;


    }
    return true;

}


/* Count the number of arg the operation needs. */
int instArgsCount(int instInd)
{
	int i, count = 0;

	for(i = 0; i < NUM_ARGS_ADRS; i++)
		if (ICS[instInd].addType[i][0] != NULL_METHOD)
			count++;

	return count;
}


/* return the instruction index in the ICS global struct array */
int getInstructionI(char *cmd)
{
    int i;

    /* loop  loop through all instruction commands global struct array */
    for(i = 0; i < NUM_ARGS_ADRS; i++)
        if(!strcmp(cmd, pSpLine -> cmd))
            return i;
    
    return -1; /* no such instruction command, safety major */
}


/* compute rather arg is compitable with the curent line command capabilities */
boolean comptInstArg(char *arg)
{
    if(!arg)
        return false;
    
    return true;
}


/* Checks whether a given parameter is a number. */
boolean isNumber(char *arg)
{
	char *p = arg; 

    if(!arg)
        return false;

	if(*p != '+' && *p != '-' && !isdigit(*p)) /* Number can have a sign of + or -, it's absolute. */
		return false;

	for(p++; *p; p++)
		if(!isdigit(*p))
			return false;

	return true;
}


/* 
* Checks if arg num is in range for instruction commands: 
* for .data should be between -16384 - 16383 (15bits), 
* for rest -2048 - 2047 (12bits).
*/
boolean numInRange(char *num)
{
    signed long tmp;

    if(!num)
        return false;

    tmp = atoi(num);

    if(!tmp)
        return false;

    if(!strcmp(pSpLine -> cmd, DIR_DATA))
    {
        if(tmp < -16384 || tmp > 16383)
            return false;
    }

    else
    {   
        if(tmp < -2048 || tmp > 2048)
            return false;
    }

    return true;
}


/* rather an arg is a reg type */
boolean isReg(char *arg)
{
    char *p;

    if(!arg)
        return false;
    
    if(!(
      strcmp(arg,"r0") && strcmp(arg,"r1") && strcmp(arg,"r2") && strcmp(arg,"r3") && strcmp(arg,"r4") &&
      strcmp(arg,"r5") && strcmp(arg,"r6") && strcmp(arg,"r7") 
      ))
        return false;

        
return true;
    
}