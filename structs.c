
#include "structs.h"



/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                LABEL STRUCTS 
***************************************************************************************************************
\*                                                                                                           */


/* 
* Add a new label to the label-table from global splitted line var (pointer) fetched data
* This func is being called after full line parse has commited, therefore, line is 100% legit 
* and SplittedLine global var has all the paramaters assigned to its struct members 
*/
void addLabel(char *name, labelType type, int value)
{
	labelNode *p; 

	p = (labelNode *)safeAlloc(sCalloc, 1, sizeof(struct labelNode)); /* allocate mem for 1 lbl node */
   *p = (labelNode){0}; /* double making sure all struct fields will be reset to 0/null (on top of using calloc()) */

   p -> name = (char *)safeAlloc(sMalloc, strlen(name)+1); /* +1 for the null-terminator */
   p -> name = strcpy(p -> name, name); /* copy label */  
   p -> type = type;
   p -> value = value;
   
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


/* Finds a label in the label table by 'name' */
boolean findLabel(char *name)
{
	labelNode *p;

   if(!lblHead)
      return false;

	for(p = lblHead; p; p = p->next)
		if(!strcmp(p -> name, name))
      {
         numOfErrors++;
         printError(LABEL_EXIST ,name);
			return true;
      }

	return false;
}

/* Updates the label table */
void updateLblTable()
{
	labelNode *p;

	for (p = lblHead; p; p = p->next)
	{
		if (p -> type == L_DATA || p -> type == L_STRING)
			p-> value += (IC +  STARTING_ADDRS); /* increase each .data/.string label value with sum file IC + 
                                            STARTING_ADDRS (100) */
      else if(p -> type == L_INST)
         p -> value +=  STARTING_ADDRS; /* increase each instruction label with STARTING_ADDRS (100) 
                                        that is assumed as the starting mem program loaded address */
	}
}


/* Returns the given symbol type. */
labelType getLabelType(labelNode *label)
{
	if(label)
		return label -> type;
	return UNDEFINED_LABEL;
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

/*                                                                                                           *\
***************************************************************************************************************                                                                                                         
                                                WORD STRUCTS 
***************************************************************************************************************
\*                                                                                                           */


/* compute how much memory (word) each instruction operand requires, 0.5 - reg, others 1 */
float opMemReq(char *op)
{
	if(whichAddArgType(op) == DIRECT_REG || whichAddArgType(op) == REF_REG) /* DIRECT_REG - 'r0-7', REF_REG '*r0-7' type */
   {
      printf("this arg requires 0.5\n"); /* debug */
      return 0.5; /* debug */
   }

   printf("this arg requires 1.0\n");
	return 1.0; /* IMMIDIET - '#', DIRECT - 'label address' requires 1 */
}

/* compute how much memory (word) input instruction line requires, default is 1 as we always need at least 1 */
float instLineMemReq()
{
   float wordsCnt = 1.0; /* words counter */
   argNode *p; /* run on line arguments */

   if(!pSpLine) /* safety major */
      return 0; 

   for(p = pSpLine -> argsHead; p; p = p -> next) /* Run on the args, max run is 2 as max args for instruction is 2 */
			wordsCnt += opMemReq(p -> name); /* sum the words required based on arguments from line */

   printf("This Instruction requires: %f memory slots\n", wordsCnt); /* debug */

   return wordsCnt;
}


/* increase global var IC by how much current instruction line memory requirement */
void increaseIC()
{
   IC += ceil(instLineMemReq()); /* we use ceil() for cases such 'add r1, #-3',
                                 instLineMemReq() will preduce 2.5 words, though
                                 actually requires 3 words */
}


/* increase global var DC by size of .data/.string argument size */
void increaseDC()
{
   labelNode *p; 

   if(!strcmp(pSpLine -> cmd, DIR_DATA)) /* .data, increase by line argument counter */
      DC += pSpLine -> numArgs;

   else
   {
      p = pSpLine -> argsHead; /* case .string, we know argsHead exist */
      DC += strlen(p) + 1; /* +1 for null-terminator */ 
   }
}


/* will create the first word for instruction commands */
void firstWord()
{

}