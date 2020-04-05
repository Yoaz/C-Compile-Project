
/* parse .data & instruction command arguments ISSUE: when .data odd num of arguments + error (illegal comma, not a number argument -> segmentation error) */
boolean parseCmd(char *restOfLine)
{
    if(!fetchArgs(restOfLine)) /* syntax issue with input line arguments segment */
        return false;

    if(pSpLine -> numArgs == 0) /* quick check for unique case of 0 args from line */
    {
    /* check with commands: 'stop', 'jsr' - that requires no arguments at all */
    if((!strcmp(pSpLine -> cmd, "stop") || !strcmp(pSpLine ->  cmd, "rts")))
            return true;

    /* else, rest of commands requires at least 1 args, error printing corresponded to command type */
    numOfErrors++;
    if(!strcmp(pSpLine -> cmd, DIR_DATA))
        printError(MISSING_DATA);
    else    
        printError(MISSING_OPERAND);
    return false;
    }}

    
    /* arguments legit check for .data directive or instruction commands that requires at least 1 operand accordingly */
    for (arg = pSpLine -> argsHead; arg; arg = arg -> next)  
    {
        if(!strcmp(pSpLine -> cmd, DIR_DATA))
        {
            if(!legitDataArg(*arg)) /* check arg is a valid number and in range for .data command */
                return false;
        }

        /* else it is an insturction command (exclude stop and jsr) */
        if(!legitInstArg(*arg) || !comptInstArg(*arg)) /* check legit instruction arg syntax and compitable with current command arg */
            return false; 
    }

    /* check match num args provided in line with command accepted num of args */
    
}




boolean legitDataArg(char *arg)
{

    if(!arg) /* safety major */
        return false;
    
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

    return true;
}


boolean fetchArgs(char *restOfLine)
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
        return true;
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
                    puts("ISSUE ADDING ARG TO LIST")
                    exit(1));
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

	if(status == waitForArg) /* There is a separator at the end of the line */
	{
		printf("Illegal SEPARATOR. \n");
        
		return false;
	}

	else(status == readArg) /* There is arg at the end of the line */
    {
        /* copy the arg to an allocated "string" */
        argLen = i - startI;
        temp = (char *)	safeAlloc(sCalloc, (argLen + 1), sizeof(char));	/* i is the place in the line where the arg ends. */
        temp = strncpy(temp, restOfLine + startI, argLen); /* +1 for the null-terminator */
        temp[argLen] = '\0';	/* Adds the null terminator to the end of the line. */
        if(!addArgToArgList(temp))
        {
            free(temp);
            puts("ISSUE ADDING ARG TO LIST")
            exit(1));
        }

        argLen = 0;
        ++argc;
        free(temp);
    }

    pSpLine -> numArgs = argc; /* assign num of args to global splitted var */

	return true;
}
