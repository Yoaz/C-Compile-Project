                    /*** IN THIS FILE WE WILL HAVE ALL THE FUNCTION RELATED TO DEALING WITH FILES
                                 CREATING FILES, FETCHING LINES FROM FILES ETC               ***/

#include "fileHandler.h"


/* The function read char by char from file and assign to line when \n occur */
FILE *fetchFile(char *fileName, char *fileMode)
{
    FILE *fp;

    if(!(fp = fopen(fileName, fileMode) )) /* Check success of file open */
    {
        fprintf(stderr, "Failed Loading File or File Not Exist"); /* TODO: add to internal error buffer */
        return NULL;
    }
    return fp;
}








/* The function read char by char from file and assign to line when \n occur */
void fetchLine(FILE *fd, char **line)
{
   int ch, size=1, len=0, inString = false, firstQuote = false;

    /* Assign 'line' with memory big enough for 1 char and end string char '\0' */
    (*line)=(char *)realloc(NULL,sizeof(char)*size);
    if(!(*line))
    {
        fprintf(stderr, "Couldn't allocate memory!");
        exit(0);
    }
   
      
    /* as long as not end of file */
    while((ch=fgetc(fd)) != EOF)
    { 
        /* Is begining of string of .string directive */
        if(ch == (char)STRING_START)
        {
            /* If previous char in line is not backslash escape char && not firstQuote */
            if((*line)[len-1] != ESCAPED_BACKSLASH && firstQuote)
            inString = false;
            /* first occurence of ", begining of string */
            if(!firstQuote)
        {
            inString = true;
            firstQuote = true;
            }
        }

        if( (ch == (char)COMMA) && !inString )
        {
            
        }

        /* On the fly replacing multiple inputed white chars (exclude '\n') with one space char (' '), 
        * if @ begining of string, avoid all white chars inserted */
        if(whiteCh(ch) && !inString)/* check if current char is of white char (exclude '\n') */
        { 
            if(len-1<0) /* if we are @ begining of setntence */
                continue; /* avoid the white char and move along to scan next one */
            if(ch != SPACE) /* if current white char isn't a single space */
                ch=SPACE; /* replace with single space char */
            if(whiteCh((*line)[len-1])) /* if previous string char is also of white char, avoid and continue */ 
                continue;
        }
   
        /* if 'enter' ('\n') inserted */
        if(ch == LINE_BREAK)
        {
            /* Avoiding extra white chars at end of file string before '\n' */
            while(whiteCh((*line)[--len]) && len > 0)
            {
                (*line)-1; 
            } 
            (*line)[len+1]=STRING_END;  /* assign end of string char */
            return;
        }
      
        (*line)[len++]=ch;  /* add the char to string */

        if(len==size)   /* if we need more memory allocation */
        {
            (*line) = realloc((*line),sizeof(char)*(size+=1));
            if(!(*line))
            {
                fprintf(stderr, "Couldn't allocate memory!");
                exit(1);
            }
        }
   }
}