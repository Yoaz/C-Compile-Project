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

	if (!line || *line == COMMENT_SIGN) /* ignore undefined/comments lines */
		return false;

	for (p = line; *p; p++)
		if (!isspace(*p)) /* The line isn't empty */
			return true;

	return false;
}


/*                                                                                                           *\
********************************************* LABLE PARSING ***************************************************
\*                                                                                                           */


/* Checks whether the given label is valid. */
boolean isValidLabel(char *label, int len)
{
	char *p= label; /* helper */

	/* List of rules */
	if (!label || !isalpha(*p) || len > LABLE_SIZE || findRegsiter(label) != NULL_REG || getIntructionId(label) != UNDEFINED_IID || isMachineGuidance(label))
		return false;

	for (p++; *p; p++)
		if (!isalnum(*p)) /* Label should be alpha numeric! */
			return false;

	return true;
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