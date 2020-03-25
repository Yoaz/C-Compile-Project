#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#include "globals.h"
#include "errors.h"



extern int IC; /* instructions counter */
extern int DC; /* data counter */
extern int numOfErrors; /* number of errors */
extern spLine *pSpLine;



typedef struct labelNode{
   char name; /* label name */
   char type[TYPE_SIZE]; /* label type */
   int val; /* value of DC */
   struct labelNode *next; /* pointer to the next label */
   
}labelNode;

labelNode *labelTable[NUMBER_OF_LABELS];



/* functions prototype */
void labelCheck(char*);
void checkSyntax(char*);
void Availability(char*);
int checkExist(char*);
void creatLableTable(char*,int);
void addLabel(char*,int);
void insertData(labelNode*, char*);
void insertLabel(char*);
void printlblTable(void);
void instuctionType(char*);
void freeAll(void);
int typeCheck(char*);

#endif