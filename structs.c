
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



/* The function checks in the label-table if there an existance label */
int checkExist(char *label)
{
   int i;
   labelNode *curr; /* row */
   for(i=0;i<4;i++)
   {
      if(labelTable[i] == NULL) /* table is not set */
         return 1; /* no table */
      curr = labelTable[i]; /* set the first row */
      while(curr != NULL)
      {
         if(strcmp(label, curr->name))
            curr = curr->next; /* next row */
         else
         return -1; /* found existance */
      }
   }
   strcpy(pSpLine -> label, label); /* save the current label , passed all checks */
   return 0; /* no existance */
}


void instructionLable(char *line)
{
   char *token;
   token = strtok(line," "); /* cut the type of instruction */
   if(isspace(token[strlen(token)-1])) 
      token[strlen(token)-1] = '\0'; /* cut the blanks */
   insertLabel(token);
   
}

/* this function insert the new label to the table */
void insertLabel(char* type)
{  
   int typeNum = typeCheck(type);
   if(labelTable[0] == NULL)
      creatLableTable(type,typeNum); /* creat new label table */
   else
      addLabel(type, typeNum); /* insert the curretn label to the exist label table */ 
}


/* this function creat label table and insert the new label */
void creatLableTable(char *theType, int Numtype)
{
   labelTable[0] = (labelNode*)(malloc(sizeof(labelNode)));
   switch (Numtype)
   {
      case 1: /* .data */
      {
        /* data(currentLine+ind); */
         break;
      }
      case 2: /* .string */
      {
         break;
      }
      case 3: /* .entry */
      {
         break;
      }
      case 4: /* extern */
      {
         break;
      }
   }
   
   if(labelTable[0] == NULL)
   {
      fprintf(stderr,"Memory allocation failed\n");
      exit(-1);
   }
   strcpy(labelTable[0]->name, pSpLine -> label);
   strcpy(labelTable[0]->type, theType);
   labelTable[0]->next = NULL;
}

void addLabel(char *theType, int type)
{
   int i;
   labelNode *temp, *node;
   
   for(i=0;labelTable[i];i++)
   {
      temp = labelTable[i];
      if(!strcmp(labelTable[i]->type,theType))
      {
         while(labelTable[i]->next)
            temp = temp->next;
         node = (labelNode*)(malloc(sizeof(labelNode)));
         if(node == NULL)
         {
            fprintf(stderr,"Memory allocation failed\n");
            exit(-1);
         }
         strcpy(node->name, pSpLine -> label);
         strcpy(node->type, theType);
         temp->next = node;
         node->next = NULL;         
         return;
      } 
   }
   node = (labelNode*)(malloc(sizeof(labelNode)));
   if(node == NULL)
   {
      fprintf(stderr,"Memory allocation failed\n");
      exit(-1);
   }
   strcpy(node->name, pSpLine -> label);
   strcpy(node->type, theType);
   labelTable[i] = node;
   labelTable[i]->next = NULL;            
}

void printlblTable(void)
{
   labelNode *curr;
   int i;
   for(i=0;labelTable[i];i++)
   {
      curr = labelTable[i];
      while(curr)
      {
         printf("name: %s, type: %s \t",curr->name,curr->type);
         curr = curr->next;
      }
      putchar('\n');
         
   }
}

/* this function free all the allocation memory in the program */
void freeAll(void)
{
   labelNode *prev, *curr;
   int i;
   for(i=0;labelTable[i];i++)
   {
      prev = labelTable[i];
      curr = prev->next;
      while(curr)
      {
         free(prev);
         prev = curr;
         curr = curr->next;
      }
      free(prev);
   }
}

   
