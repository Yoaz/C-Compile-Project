#include <stdio.h>
#include "data.h"

/* The file extentions */
#define SOURCE_EXT ".as"
#define OBJ_EXT ".ob"
#define ENT_EXT ".ent"
#define EXT_EXT ".ext"


/* The file open mode */
#define READ_ONLY "r"
#define READ_WRITE "r+"
#define WRITE_ONLY "w"
#define WRITE_READ "w+"
#define UPDATE_ONLY "a"
#define UPDATE_READ "a+"


/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*  

/* will fetch line from current FILE * and will parse according to the line content in case of no errors */
void fetchLine(FILE *, char**);