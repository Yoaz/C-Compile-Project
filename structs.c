
#include "structs.h"








/*                                                                                                           *\
********************************************* LABLE STRUCTS ***************************************************
\*                                                                                                           */


/* Add a new symbol to the symbol-table from global splitted line var (pointer) fetched data */
void addLabel()
{
	labelNode *p; 

	p = (labelNode *)safeAlloc(sCalloc, 1, sizeof(struct labelNode)); /* allocate mem for 1 lbl node */
   *p = (labelNode){0}; /* double making sure all struct fields will be reset to 0/null (inspite of using calloc()) */

   p -> name = (char*)safeAlloc(sMalloc, strlen(pSpLine -> label)+1); /* +1 for the null-terminator */
   p -> name = strcpy(p -> name, pSpLine -> label); /* copy label name from splitted line pointer */
   /* TODO: complete this assigning values node 
   p -> type = type;
   p -> value = value;
   */
   
   /* table is empty */
   if(!lblHead)
   {
      lblHead = p; /* p first label table node, point label table head to it */
      lblLast = p; /* p first and last label table node, point label table last to it */
      return;
   }

   /* table is not empty */
   lblLast -> next = p; /* last label is new label */
   lblLast = p; /* p last label table node, point label table last to it */

   return;
}

/* Finds a label in the label table by 'name'. */
boolean findLabel(char *name)
{
	labelNode *p;

   if(!lblHead)
      return false;

	for(p = lblHead; p; p = p->next)
		if(!strcmp(p -> name, name))
      {
         printError(LABEL_EXIST ,name);
			return true;
      }

	return false;
}

/* Updates the symbol table addresses. */
void updateLabelTable(FILE *fp, int inc)
{
	labelNode *p;

	for (p = lblHead; p; p = p->next)
	{
		if (p -> type == CODE)
			p -> value += inc;
		else if (p -> type == DATA)
			p-> value += IC + inc;
	}
}

/* Returns the given symbol type. */
labelType getLabelType(labelNode *label)
{
	if(label)
		return label -> type;
	return UNDEFINED_SYMBOL;
}

/* Returns the given symbol value.  */
long getSymbolVal(labelNode *label)
{
	if (label)
		return label -> value;
	return 0;
}

/* will free current global label list */ 
void freeLblTable()
{
   labelNode *p;
   
   if(!lblHead)
      return;
      
   p = lblHead -> next;

   while(lblHead)
   {
      free(lblHead->name); /* release allocated mem for name field of label node */
      free(lblHead); /* release whole node */
      
      if(!p) /* only one node in lbl table, break */
         break;

      lblHead = p; /* move lblHead to point next node in table (if exist) */
      p = p -> next;
   }
   
   lblLast = NULL;

    return;
}


/* Debug */
void printLblTabel()
{
	labelNode *p;

   if(!lblHead){
      puts("\nNO LABELS IN THIS FILE\n");
      return;
   }

	printf("\n\n\t***** Label-Table ***** \n\n");
	for (p = lblHead; p; p = p->next)
		printf("Name = %s \tType = %d\t Value = %li\n", p->name, p->type, p->value); /* TODO: Add p -> type, p -> value */
	printf("\n\t*** End Of Label-Table *** \n\n\n");
}

/* Debug */
void printArgTabel()
{
	argNode *p;

   if(!pSpLine || !pSpLine -> argsHead)
   {
      puts("\nNO ARGS IN THIS COMMAND\n");
      return;
   }
      

   printf("ARGS-LIST --> ");
	for (p = pSpLine -> argsHead; p; p = p->next)
		printf("Name = %s ", p->name); /* TODO: Add p -> type, p -> value */
   printf("\tNum of args: %d", pSpLine -> numArgs);
   
   puts("\n");
   
}