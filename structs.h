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
   char *name; /* label name */
   char *type; /* label type */
   int value; /* value of DC */
   struct labelNode *next; /* pointer to the next label */
   
}labelNode;

/* global label tabel list head */
labelNode *lblHead;

/* last inserted label to save unesery list loop on new label insert */
labelNode *lblLast;


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

#endif