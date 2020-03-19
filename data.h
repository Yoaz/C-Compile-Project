

#define COMMA ','
#define IMMEDIATE_ADD_SIGN '#'
#define COMMENT_SIGN ';'
#define COLON ':'
#define SPACE ' '
#define STRING_END '\0'
#define DOT '.' 
#define STRING_START '"'
#define ESCAPED_BACKSLASH '\\'
#define LINE_BREAK '\n'

#define MAX_FILE_NAME 30
#define MAX_LINE_SIZE 81
#define ERROR_SIZE 200
#define LABEL_SIZE 31
#define TYPE_SIZE 8
#define NUMBER_OF_LABELS 4
#define OPCODES 15 /* added */

/* define boolean variable */
typedef enum boolean{false, true} boolean;

/* system's available registers */
enum registers{r0, r1, r2, r3, r4, r5, r6, r7};


