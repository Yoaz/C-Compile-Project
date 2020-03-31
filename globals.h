#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h> /* for NULL recognize */

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

#define DIR_DATA ".data"
#define DIR_STRING ".string"
#define DIR_ENTRY ".entry"
#define DIR_EXTERN ".extern"

/* define boolean variable */
typedef enum boolean{
    false, 
    true
}boolean;

/* system's available registers */
enum registers{NULL_REG= -1, RO, R1, R2, R3, R4, R5, R6, R7};

/* ARE */
typedef enum
{
	A_ABSOLUTE,
	E_EXTERNAL,
	R_RELOCATABLE
} ARE;

/* Types of instruction command line arguments */
typedef enum
{
	NULL_METHOD = -1,
	IMMEDIATE,
	DIRECT,
	DIRECT_REG,
	REF_REG,
	ADD_METHOD_COUNT
} AddMethod;

typedef enum
{
	UNDEFINED_SYMBOL = -1,
	CODE,
	DATA,
	STRING,
	ENTRY,
	EXTERNAL
} labelType;

/* enum for different instruction commands */
typedef enum
{
	UNDEFINED_CMD = -1,
	MOV = 0000,
	CMP = 0001,
	ADD = 0010,
	SUB = 0011,
	LEA = 0100,
	CLRL = 0101,
	NOT = 0110,
	INC = 0111,
	DEC = 1000,
	JMP = 1001,
	BNE = 1010, 
	RED = 1011,
	PRN = 1100,
	JSR = 1101,
	RTS = 1110,
	STOP = 1111
} cmdType;

/* arguments from each line stored in list arguments */
typedef struct argNode
{
	char *name;
	struct argNode *next;
}argNode;

/* split line object */
typedef struct
{
    boolean lblFlag; /* flag if current line has label, default = false */
    char *label; /* if there is no lable, then NULL */
	char *cmd; /* will hold the command in current line */
	argNode *argsHead;	/* Pointer to the head of arguments object (if there isn't, then NULL) */ 

}spLine;

/* pointer to split line object */
spLine *pSpLine;





#endif



