#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h> /* for ceil() use etc */
#include <stdlib.h>


#include "globals.h"
#include "errors.h"

#define WORD_CHAR_SIZE 2 /* array of 2 chars = 16bits, word require 15bits, 1 extra */
#define ARE_BITS 3 /* abseloute, rellocated, exernal bits count */
#define OP_TYPE_BITS 4 /* same for dest/src op */
#define OPCODE_BITS 4 /* opcode bits count */

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
   char *type; /* label type */
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

#define bitOn(arr,k)     ( arr[(k/8)] |= (1 << (k%8) ) ) /* turn bit on in array by index of bit - macro */
#define bitOff(arr,k)   ( arr[(k/8)] &= ~(1 << (k%8) ) /* turn bit off in array by index of bit - macro */
#define checkBit(arr,k)  ( arr[(k/8)] & (1 << (k%8)) ) /* check bit status in array by index of bit - macro */ 


/* define word/one line translate db node - 15bits (1 bit extra, since we have char[2] = 16 bits) */
typedef union word{
   char opcodeWord[WORD_CHAR_SIZE]; /* first word in each line translation */
   char srcOp[WORD_CHAR_SIZE]; /* source operand from line if exist */
   char destOp[WORD_CHAR_SIZE]; /* destenation operand from line if exist */  
}word;





/* functions prototype */
void labelCheck(char*);
void checkSyntax(char*);
int checkExist(char*);
void updateLabelTable(FILE *, int);
void addLabel();
void insertLabel(char*);
void printLblTabel();
labelType getLabelType(labelNode *);
long getSymbolVal(labelNode *);
void freeLblTable();
boolean findLabel(char *);
void printArgTabel();

#endif