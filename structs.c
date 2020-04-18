
                    /*** IN THIS FILE WE WILL HAVE ALL THE MAIN 
                                DATA BASES/STRUCTURES      ***/



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
labelNode *findLabel(char *name)
{
	labelNode *p;

   if(!lblHead)
      return NULL;


	for(p = lblHead; p; p = p->next)
		if(!strcmp(p -> name, name))
      {
			return p;
      }

	return NULL;
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
   
   lblHead = NULL;
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


/*********************** general **********************/

/* recieve a bits char array and return the value of the word in a signed int */
int binCharArrToDec(char word[])
{
   int i;
   signed short int result = 0; /* short since we are having each word in size 15 bits */

   for(i=0; i <WORD_TOTAL_BITS; i++)
   {
      result = (result << 1) | (word[i] == '1'); /* set bits in result according to char array '1', '0' */
   }
   
   if(word[0] == '1') /* case of negetive number, set last bit in result to be 1 */
   {
      setBit(result, (sizeof(result) * 8) - 1);
   }

   /* debug */
   for(i=0; i <(sizeof(result) * 8); i++)
   {
      if(checkBit(result,i))
         printf("-%d", i);
   }
   printf("\n");

   return result;
}


/* Debug */
void printWord(dtWord *memWord)
{
	int i;
	for (i = 0; i < WORD_TOTAL_BITS; i++)
		printf("%c", memWord -> word[i]);

	puts("");
}


/*********************** instruction **********************/

/* increase global var IC by how much current instruction line memory requirement */
void increaseIC()
{
   IC += ceil(instLineMemReq()); /* we use ceil() for cases such 'add r1, #-3',
                                 instLineMemReq() will preduce 2.5 words, though
                                 actually requires 3 words */
}


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


/* Creates the first instruction word in memory. */
instWord *setFirstInstWord(int opCode, argAddType src, argAddType dest, ARE are)
{
	instWord *newWord = (instWord *)safeAlloc(sCalloc, 1, sizeof(instWord));
	unsigned long mask;
	int i, ind = (WORD_TOTAL_BITS - 1); /* start from the last index -> binary works backwards */
					  

	/* Converts the fields to binary */
	for (i = 0, mask = 1; i < ARE_BITS; i++, mask <<= 1)
		newWord -> word[ind--] = mask & are ? '1' : '0';
	for (i = 0, mask = 1; i < OP_BITS; i++, mask <<= 1)
		newWord -> word[ind--] = mask & dest ? '1' : '0';
	for (i = 0, mask = 1; i < OP_BITS; i++, mask <<= 1)
		newWord -> word[ind--] = mask & src ? '1' : '0';
	for (i = 0, mask = 1; i < OPCODE_BITS; i++, mask <<= 1)
		newWord -> word[ind--] = mask & opCode ? '1' : '0';

	return newWord;
}


/* creates instruction argyment word in memory */
instWord *setInstArgWord(int value, ARE are, argAddType argType, ...)
{
	instWord *newWord = (instWord *)safeAlloc(sCalloc, 1, sizeof(instWord));
	unsigned long mask;
	int i, ind = (WORD_TOTAL_BITS - 1); /* start from the last index -> binary works backwards */
   va_list ap;
   addType adType; /* either src, either dest */

   /* set the word depend on the type of instruction argument passed */				  
   switch (argType)
   {
   
      /* number or label -> both the same in the manner of 12 bits for value, 3 bits for ARE */
      case DIRECT:
      case IMMEDIATE:
         /* Converts the fields to binary */
         for (i = 0, mask = 1; i < ARE_BITS; i++, mask <<= 1)
            newWord -> word[ind--] = mask & are ? '1' : '0';
         for (i = 0, mask = 1; i < (WORD_TOTAL_BITS - ARE_BITS); i++, mask <<= 1)
            newWord -> word[ind--] = mask & value ? '1' : '0';
         break;
      /* register either by ref or direct -> both the same in the manner of 3 bits for value, 3 bits for ARE */
      case DIRECT_REG:
      case REF_REG:
         /* if register, either by direct or by reference, 
         * one more variable is passed to this func which is 
         * either arg type is source or destination type of arg */
         va_start(ap, argType);
         adType = va_arg(ap, addType); /* fetch the extra argument passed to this func */

         if(adType == ADD_DEST) /* register or refference register as DESTINATION argument */ 
         {
            /* Converts the fields to binary */
            for (i = 0, mask = 1; i < ARE_BITS; i++, mask <<= 1)
               newWord -> word[ind--] = mask & are ? '1' : '0';
               ind -= REG_BITS; /* to reach bits 6-8 and keep bits 3-6 empty(0 as used calloc) */
            for (i = 0, mask = 1; i < REG_BITS; i++, mask <<= 1) /* save reg num in bits 6-8 */
               newWord -> word[ind--] = mask & value ? '1' : '0';         
         }
         
         else if(adType == ADD_SRC) /* register or refference register as SOURCE argument */
         {
            /* Converts the fields to binary */
            for (i = 0, mask = 1; i < ARE_BITS; i++, mask <<= 1)
               newWord -> word[ind--] = mask & are ? '1' : '0';
            for (i = 0, mask = 1; i < REG_BITS; i++, mask <<= 1) /* save reg num in bits 3-6 */
               newWord -> word[ind--] = mask & value ? '1' : '0';     
          }
          newWord -> isReg = true; /* set flag register for cases of reg in src and in dest */
   }

	return newWord;
}


/* will add new instruction word node to end of instruction list */
void addInstWordToInstList(instWord **newWord)
{
   int i;

   if(!*newWord) /* safety major */
      return;


   if(!instLstHead) /* list is empty */
   {
      instLstHead = *newWord; /* inst list head points to the new inst word */
      instLstLast = *newWord; /* also this is last int list node */
      return;
   }

   /* list isn't empty, check if last node is register and current new node is register as well */
   if((instLstLast -> isReg) && ((*newWord) -> isReg)) /* last node is a register and current new node is register */
   {
      printf("DDDDDDD IS REG!!!!!\n");
      /* implement 'or' bitwise on two words in order to merge words */
      for (i = 0; i < WORD_TOTAL_BITS; i++)
         if (instLstLast -> word[i] || (*newWord) -> word[i])
            instLstLast -> word[i] = '1';
            instLstLast -> isReg = false;
            free((*newWord)); /* it was merge with last instruction node with list, so free */
            return;
   }
   
   /* else not case of reg as src and as dest */
   instLstLast -> next = *newWord; /* connect new word to the end of the list */
   instLstLast = *newWord; /* move last inst node list pointer to new attached node */

}


/* will free current global instruction words list */ 
void freeInstList()
{
   instWord *p;
   
   if(!instLstHead)
      return;
      
   p = instLstHead -> next;

   while(instLstHead)
   {
      free(instLstHead); /* release node */
   
      if(!p) /* only one node in lbl table, break */
         break;

      instLstHead = p; /* move lblHead to point next node in table (if exist) */
      p = p -> next;
   }
   
   instLstHead = NULL;
   instLstLast = NULL;

    return;
}


/*********************** data **********************/

/* increase global var DC by size of .data/.string argument size */
void increaseDC()
{
   labelNode *p; 

   if(!strcmp(pSpLine -> cmd, DIR_DATA)) /* .data, increase by line argument counter */
      DC += pSpLine -> numArgs;

   else
   {
      p = pSpLine -> argsHead -> name; /* case .string, we know argsHead exist */
      DC += (strlen(p) - 2) + 1; /* -2 to exclude quatation marks (""), +1 for null-terminator */
   }
}


/* creates data word node */
dtWord *setDataWord(int arg)
{
	dtWord *newWord = (dtWord *)safeAlloc(sCalloc, 1, sizeof(dtWord));
	unsigned long mask;
	int i, ind = (WORD_TOTAL_BITS - 1); /* from last index -> binary works backwards */
					
	/* Converts the fields to binary */
	for (i = 0, mask = 1; i < WORD_TOTAL_BITS; i++, mask <<= 1)
		newWord -> word[ind--] = mask & arg ? '1' : '0';

	return newWord;
}


/* will add new data word node to end of data list */
void addDtWordToDtList(dtWord **newWord)
{

   if(!*newWord) /* safety major */
      return;


   if(!dataLstHead) /* list is empty */
   {
      dataLstHead = *newWord; /* data list head points to the new data word */
      dataLstLast = *newWord; /* also this is last data list node */
      return;
   }

   dataLstLast -> next = *newWord; /* connect new data word to the end of the list */
   dataLstLast = *newWord; /* move last data node list pointer to new attached node */

}


/* will free current global data words list */ 
void freeDtList()
{
   dtWord *p;
   
   if(!dataLstHead)
      return;
      
   p = dataLstHead -> next;

   while(dataLstHead)
   {
      free(dataLstHead); /* release node */
   
      if(!p) /* only one node in lbl table, break */
         break;

      dataLstHead = p; /* move lblHead to point next node in table (if exist) */
      p = p -> next;
   }
   
   dataLstHead = NULL;
   dataLstLast = NULL;

    return;
}
