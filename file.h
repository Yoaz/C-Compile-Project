#include <stdio.h>

#include "globals.h"
#include "errors.h"

/* The file extentions */
#define SOURCE_ET ".as"
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


extern char *fname;

/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*  

/* will fetch line from current FILE * and will parse according to the line content in case of no errors */
void fetchLine(FILE *, char **);

void writeEntry(char *, char *, int *);

FILE *fetchFile(char *, char *);