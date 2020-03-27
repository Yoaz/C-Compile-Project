                    /*** THIS IS THE GLOBALS FILE, HERE WE WILL HOLD ALL PROGRAM GLOBALS
                                           VARS AND STRUCTCS DECLARATION            ***/

#ifndef GLOBALS_C
#define GLOBALS_C

#include "globals.h"


int IC;
int DC;
int numRow;
int numColumn;
int numOfErrors;
int successFiles; /* will count how many files went through the full assembler proccess with no errors */
spLine *pSpLine; /* global splitted line variable */



/* define struct to hold opcodes and their binary value */
struct operations{
    char *name;
    short binVal;
}opcodes[OPCODES] = {
    {"mov", 0000}, {"cmp", 0001}, {"add", 0010}, {"sub", 0011},
    {"lea", 0100}, {"clr", 0101}, {"not", 0110}, {"inc", 0111},
    {"dec", 1000}, {"jmp", 1001}, {"bne", 1010}, {"red", 1011},
    {"prn", 1100}, {"jsr", 1101}, {"rts", 1110}, {"stop", 1111}
};

#endif