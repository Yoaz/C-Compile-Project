#ifndef GLOBALS_H
#define GLOBALS_H

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
#define TOKEN_DELIM " \n" /* The splitter between the label, the operation, and the args */

#define MAX_FILE_NAME 30
#define MAX_LINE_SIZE 81
#define MAX_LABEL_SIZE 31
#define TYPE_SIZE 8
#define NUMBER_OF_LABELS 4
#define OPCODES 15 

/* define boolean variable */
typedef enum boolean{
    false, 
    true
}boolean;

/* system's available registers */
enum registers{NULL_REG= -1, RO=0, R1, R2, R3, R4, R5, R6, R7};

/* split line object */
typedef struct{
    boolean lblFlag; /* flag if current line has label */
    char *label; /* if there is no lable, then NULL */

}spLine;

/* pointer to split line object */
spLine *pSpLine;


#endif



