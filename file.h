#include <stdio.h>

#include "globals.h"
#include "errors.h"

/* The file extentions */
#define SRC_ET ".as"
#define OBJ_ET ".ob"
#define ENT_ET ".ent"
#define EXT_ET ".ext"


/* The file open mode */
#define READ_ONLY "r"
#define READ_WRITE "r+"
#define WRITE_ONLY "w"
#define WRITE_READ "w+"
#define UPDATE_ONLY "a"
#define UPDATE_READ "a+"

typedef struct
{
    char *rawName;
    FILE *src, *ent, *ext;
}fileObject;


/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*  

/* will fetch line from current FILE * and will parse according to the line content in case of no errors */
void fetchLine(FILE *, char **);

void writeEntry(fileObject *, char *, int *);

void writeExtern(fileObject *, char *, int *);

fileObject *fetchFile(char *, char *);

void closeFile(fileObject *);