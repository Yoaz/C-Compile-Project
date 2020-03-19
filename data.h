

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
#define OPCODES 15 

/* define boolean variable */
typedef enum boolean{
    false, 
    true
}boolean;

/* enum for generic safe memerory allocation functions */
typedef enum{
    sMalloc,
    sCalloc,
    sRealloc
}alloc_t;

/* program errors shortcuts */
typedef enum{NO_FILES=1, FAILED_OPEN, LABEL_SYNTAX, LABEL_LENGTH, LABEL_DEFINED, SAVED_WORD, DIR_NO_FOUND,
             DATA_INVALID, STR_INVALID, NOT_WHOLE, INS_NO_FOUND, WRONG_NUM_PARAM, INVALID_PARAM, LINE_LENGTH,
             INVALID_LETTER, UNDEF_PARAM, UPPER_CASE} errorList;

/* system's available registers */
enum registers{r0, r1, r2, r3, r4, r5, r6, r7};


