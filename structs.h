#ifndef STRUCT_H
#define STRUCT_H

#include <string.h>
#include <ctype.h> /* for ceil() use etc */
#include <stdlib.h>


#include "globals.h"
#include "errors.h"

#define WORD_TOTAL_BITS 15 /* size of bits each word requires */
#define ARE_BITS 3 /* abseloute, rellocated, exernal bits count */
#define OP_BITS 4 /* same for dest/src op */
#define OPCODE_BITS 4 /* opcode bits count */
#define REG_BITS 3 /* register argument bits count */


extern int IC; 
extern int DC; 
extern int numOfErrors; 
extern spLine *pSpLine;


/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                LABEL STRUCTS 
***************************************************************************************************************
\*                                                                                                           */

typedef struct labelNode{
   char *name; /* label name */
   labelType type; /* label type */
   int value; /* value of DC */
   struct labelNode *next; /* pointer to the next label */
}labelNode;

/* global label tabel list head */
labelNode *lblHead;

/* last inserted label to save unessecry list loop on new label insert */
labelNode *lblLast;


/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                WORD STRUCTS 
***************************************************************************************************************
\*                                                                                                           */

#define setBit(var,pos) ((var) |= (1 << (pos))) /* set bit n in int p macro */
#define checkBit(var,pos) ((var) & (1<<(pos)))  /* check bit in int status macro */
#define cleaBit(var,pos) ((var) &= (~(1) << (pos))) /* clear bit in int status macro */

/* define word for instructions type */
typedef struct instWord
{
	char word[WORD_TOTAL_BITS]; /* The word in binary */
	boolean isReg;	 /* Indicates if the word is register */

	struct instWord *next; /* Pointer to the next inst-mem-word */
}instWord;

instWord *instLstHead, *instLstLast; /* head of instruction list and last node in instruction list pointers */

/* define word for data type */
typedef struct dtWord{
   char word[WORD_TOTAL_BITS];
   struct dtWord *next;
}dtWord;

dtWord *dataLstHead, *dataLstLast; /* head of data list and last node in data list pointers */



/*                                                                                                           *\
********************************************* FUNCTION PROTOTYPE **********************************************
\*                                                                                                           */


/* label */
void updateLblTable();
void addLabel(char *, labelType, int);
void printLblTabel();
void freeLblTable();
labelNode *findLabel(char *);
void printArgTabel();

/* word */
dtWord *setDataWord(int );
float instLineMemReq();
instWord *setFirstInstWord(int , argAddType , argAddType , ARE );
instWord *setInstArgWord(int, ARE, argAddType, ...);
void addInstWordToInstList(instWord **);
void freeInstList();
void addDtWordToDtList(dtWord **);
signed int binCharArrToDec(char []);
void freeDtList();
void printWord();

#endif