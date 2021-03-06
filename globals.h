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
#define ARG_REF '*'
#define ARG_IMMIDIET '#'
#define ARG_REG 'r'

#define MAX_LABEL_SIZE 31
#define TYPE_SIZE 8
#define NUM_ARGS_ADRS 16 
#define NUM_INSTRUCTION_COMMANDS 16 

#define STARTING_ADDRS 100 /* program default loaded address */

#define DIR_DATA ".data"
#define DIR_STRING ".string"
#define DIR_ENTRY ".entry"
#define DIR_EXTERN ".extern"

/* define boolean variable */
typedef enum boolean{
    false, 
    true
}boolean;

/* ARE */
typedef enum
{
	NULL_ARE = -1,
	ABSOLUTE = 0,
	RELOCATABLE,
	EXTERNAL
} ARE;

/* Addressing types */
typedef enum
{
	ADD_SRC = 0,
	ADD_DEST = 1,
	ADD_TYPE_COUNT
} addType;

/* Types of instruction command line arguments */
typedef enum
{
	NULL_METHOD = -1,
	IMMEDIATE,
	DIRECT,
	REF_REG,
	DIRECT_REG,
	ARG_TYPE_COUNT
} argAddType;

typedef enum
{
	UNDEFINED_LABEL = -1,
	L_INST,
	L_DATA,
	L_STRING,
	L_ENTRY,
	L_EXTERNAL
} labelType;

/* define struct to hold instrucions and their accpeted args */
struct instructions
{
	char *cmd;	/* the instruction command name */
	argAddType addType[ARG_TYPE_COUNT][ARG_TYPE_COUNT + 1]; /* addressing method list */
};

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
	int numArgs; /* counter for arguments provided via line */
}spLine;

/* pointer to split line object */
spLine *pSpLine;


#endif



