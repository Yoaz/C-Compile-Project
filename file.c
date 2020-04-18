                    /*** IN THIS FILE WE WILL HAVE ALL THE FUNCTION RELATED TO DEALING WITH FILES
                                 CREATING FILES, FETCHING LINES FROM FILES ETC               ***/

#include "file.h"



/* function will fetch the file with the fileName provided and open with mode based on file mode provided */
fileObject *fetchFile(char *fileName, char *fileMode)
{
    fileObject *fileOb;
    char *tmp;

    fileOb = (fileObject *)safeAlloc(sCalloc, 1, sizeof(fileObject));

    /* save raw file name to file object structure to use later in different file creating/handling */
    fileOb -> rawName = (char *)safeAlloc(sCalloc, strlen(fileName) + 1, sizeof(char));
    strcpy(fileOb -> rawName, fileName);
    
    /* tmp file name of source, in order to open with extension */
    tmp = (char *)safeAlloc(sCalloc, (strlen(fileName) + strlen(EXT_ET) + 1) , sizeof(char));
    strcpy(tmp, fileName);
    strcat(tmp, SRC_ET);

    /* assign file handler to file structure src handler */
    fileOb -> src = fopen(tmp, fileMode);

    /* issues with opening the related file */
    if(!fileOb -> src)
    {
        printError(FAILED_OPEN, tmp); /* Will print FAILED_OPEN err */ 
        fclose(fileOb -> src);
        free(tmp); /* no need for this other than one time use */
        return NULL; /* name of file not found, move on to next file from argv[] if exist */
    }

    free(tmp);
    return fileOb;
}

/* close file and free file object stracture */
void closeFile(fileObject *fileOb)
{
    if(!fileOb)
        return;

    free(fileOb -> rawName);

    if(fileOb -> src)
        fclose(fileOb -> ent);
    if(fileOb -> ent)
        fclose(fileOb -> ent);
    if(fileOb -> ext)
        fclose(fileOb -> ent);
    if(fileOb -> obj)
        fclose(fileOb -> ent);

    free(fileOb);    

}

/* the function read char by char from file and assign to line when \n occur
* this funcion will 'on the fly' avoid extra white chars from input, and replace with since space char, as long
as input isnt part of .string " " string, for ex: (SPACE X 4)\t AB:\t\t\t .data -> will turn into: AB: .data with one
* SPACE char in between 2 words that originally inputted with white tabs in between */
void fetchLine(FILE *fd, char **line)
{
   int ch, size=1, len=0, inString = false, firstQuote = false;

    (*line)=(char *)safeAlloc(sCalloc,1, sizeof(char)*size);
      
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
            /* assign line with a new size using safeAlloc (REALLOC) */
            (*line) = (char *)safeAlloc(sRealloc,(*line),sizeof(char)*(size+=1));
        }
   }
}


/* write entry file */
void writeEntry(fileObject *fileOb, char *lbl, int value)
{
    char *fname;

    if(!fileOb -> ent) /* ent file handler isn't open already */
    {
        fname = (char *) safeAlloc(sCalloc, (strlen(fileOb -> rawName) + strlen(ENT_ET) + 1), sizeof(char));
        strcpy(fname, fileOb -> rawName);
        strcat(fname, ENT_ET);
        fileOb -> ent = fopen(fname, WRITE_ONLY); /* if already exist, rewrite as this is first file open for this program loop */
        free(fname);
    }

    fprintf(fileOb -> ent, "%s\t%d\n", lbl, value);

}


/* write extern file */
void writeExtern(fileObject *fileOb, char *lbl, int value)
{
    char *fname;

    if(!fileOb -> ext) /* ext file handler isn't open already */
    {
        fname = (char *) safeAlloc(sCalloc, (strlen(fileOb -> rawName) + strlen(EXT_ET) + 1), sizeof(char));
        strcpy(fname, fileOb -> rawName);
        strcat(fname, EXT_ET);
        fileOb -> ext = fopen(fname, WRITE_ONLY); /* if already exist, rewrite as this is first file open for this program loop */
        free(fname);
    }

    fprintf(fileOb -> ext, "%s\t%04d\n", lbl, value);

}


/* write the object file headline*/
void writeObjectHeadLine(fileObject *fileOb, int ic, int dc)
{
    char *fname;
    

    if(!fileOb -> obj) /* obj file handler isn't open already */
    {
        fname = (char *) safeAlloc(sCalloc, (strlen(fileOb -> rawName) + strlen(OBJ_ET) + 1), sizeof(char));
        strcpy(fname, fileOb -> rawName);
        strcat(fname, OBJ_ET);
        fileOb -> obj = fopen(fname, WRITE_ONLY); /* if already exist, rewrite as this is first file open for this program loop */
        free(fname);

        /* one time only write file headline "instruction file total counter   /t    data file total counter" */
        fprintf(fileOb -> obj, "\t%d\t%d\n", ic, dc);
    }

}


/* write object file */
void writeObject(fileObject *fileOb)
{
    unsigned int len, i;
    char *sNum;
    instWord *wd;
    int decWord;

    /* write to object file the headline of instructions and data count before IC reset */
    writeObjectHeadLine(fileOb, IC, DC);

    IC = STARTING_ADDRS; /* reset IC to starting address */

    if(instLstHead) /* in case there are no instruction commands at all in file */
    {
        /* no need to check if file handler of object exist as always writeObjectHeadline() called prior */
        for(wd = instLstHead; wd; wd = wd -> next) /* run on instruction word nodes */
        {   
            decWord = binCharArrToDec(wd -> word);
            i++;
            if(decWord < 0) /* case of 'negetive' word value */
            {
                sNum = safeAlloc(sCalloc, 1, 5); /* allocate mem for octal num in string format */
                sprintf(sNum, "%05o", decWord); /* copy octal digits to string */
                len = strlen(sNum);
                memmove(sNum, sNum + (len - 5), 5);
                fprintf(fileOb -> obj, "%04d\t%.05s\n", IC++, sNum); /* print to file part of the string number */
                free(sNum);
            }
            
            else /* 'positive' word value */
                fprintf(fileOb -> obj, "%04d\t%05o\n", IC++, decWord);
        }
    }
    printf("EEEEEEEEE %d\n", i);
    writeDtListToObject(fileOb); /* write data list to object file after end of instruction list */

}


/* write data list to end of object file */
void writeDtListToObject(fileObject *fileOb)
{
    dtWord *d;
    unsigned int len;
    char *sNum;

    if(!fileOb || !dataLstHead)
        return;

    for(d = dataLstHead; d; d = d -> next) /* run on args */
    {
        if(binCharArrToDec(d -> word) < 0) /* case of negetive number */
        {
            sNum = safeAlloc(sCalloc, 1, 5); /* allocate mem for octal num in string format */
            sprintf(sNum, "%05o", binCharArrToDec(d -> word)); /* copy octal digits to string */
            len = strlen(sNum);
            memmove(sNum, sNum + (len - 5), 5);
            fprintf(fileOb -> obj, "%04d\t%.05s\n", IC++, sNum); /* print to file part of the string number */
            free(sNum);
        }

        else
            fprintf(fileOb -> obj, "%04d\t%05o\n", IC++, binCharArrToDec(d -> word));
    }
}
