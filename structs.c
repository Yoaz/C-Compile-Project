
#include "structs.h"








/*                                                                                                           *\
********************************************* LABLE STRUCTS ***************************************************
\*                                                                                                           */


/* this function checks the declared label */
void labelCheck(char *label)
{
            
   strcpy(pSpLine -> label, ""); /* reset the label */
   checkSyntax(label); /* checks the syntax of the current label */
   Availability(label); /* checks if the label is one of the saved words */
   if(checkExist(label) == -1) /* checks the existance of current label */
   { /* -1 if there is existance */
         error++; /* increase number of errors */
         strcat(errorBuff, "The label \""); /* prepare the error out-put */
         strcat(errorBuff, label);
         strcat(errorBuff, "\" is already exist\n");
         errorsOut(); /* if there is an error print it */
   }
   labelFlag = 1;
}



/* Add a new symbol to the symbol-table from global splitted line var (pointer) fetched data */
void addLabel()
{
	labelNode *p; 

	p = (labelNode *)safeAlloc(sCalloc, 1, sizeof(struct labelNode)); /* allocate mem for 1 lbl node */

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

/* Finds a symbol in the symbol table by 'name'. */
boolean findLabel(char *name)
{
	labelNode *p;
   
	for (p = lblHead; p; p = p->next)
		if (!strcmp(p -> name, name))
			return true;

	return false;
}

/* Updates the symbol table addresses. */
void updateSymbolTabelAddresses(fileHandler fileH, int inc)
{
	slNode ptr;
	for (ptr = fileH->symbol; ptr; ptr = ptr->next)
	{
		if (ptr->type == CODE)
			ptr->value += inc;
		else if (ptr->type == DATA)
			ptr->value += fileH->IC + inc;
	}
}

/* Returns the given symbol type. */
symbolType getSymbolType(slNode symbol)
{
	if(symbol)
		return symbol->type;
	return UNDEFINED_SYMBOL;
}

/* Returns the given symbol value.  */
long getSymbolVal(slNode symbol)
{
	if (symbol)
		return symbol->value;
	return 0;
}

/* will free current global label list */ 
void freeLblTable()
{
   labelNode *p;

   free(lblLast); /* free global last label node which we assigned mem in intitate proccess */
   
   p = lblHead -> next;

   while(lblHead)
   {
      free(lblHead);
      lblHead = p;
      p = p -> next;
   }

    return;
}


/* Debugging */
void printSymbolTabel()
{
	labelNode *p;

	printf("\n\n\t====== Symbol-Table ====== \n\n");
	for (p = lblHead; p; ptr = p->next)
		printf("Name = %s \tType = %d\t Value = %li\n", ptr->name, ptr->type, ptr->value);
	printf("\n\t=== End Of Symbol-Table === \n\n\n");
}