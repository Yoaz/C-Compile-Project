#include "assembler.h"


int main(int c, int *argv[])
{
    FILE *fp;
    /* In case file names werent provided via command run */
    if(c <= 1)
    {
        fprintf(stderr, "File Names Must Be Provided!");
        exit(0);
    }

    while(--c)
    {
        /* fetching files */
        fetchFile(*++argv, READ_ONLY);

        if(firstRound(fp))
            secondRound(fp);
    }
    return 0; /* TODO: just a place holder for now, decide later */
}
    







