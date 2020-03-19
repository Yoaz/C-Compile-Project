#include "assembler.h"


int main(int c, int *argv[])
{
    FILE *fp;
    char *fileName;

    /* In case file names werent provided via command run */
    if(c <= 1)
    {
        fprintf(stderr, "File Names Must Be Provided!");
        exit(1);
    }

    while(--c)
    {   
        strcpy(fileName, (*filesNames)[c - (c-1)]); /* insert the file name */
        strcat(fileName,".as"); /* add the file suffix */
        
        /* fetching files */
        fetchFile(*++argv, READ_ONLY);

        if(firstRound(fp))
            secondRound(fp); /* TODO: add if(error) -> print errors, don't run secondRound */
    }
    return 0; /* TODO: just a place holder for now, decide later */
}
    







