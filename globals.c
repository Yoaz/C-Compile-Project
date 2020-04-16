                    /*** THIS IS THE GLOBALS FILE, HERE WE WILL HOLD ALL PROGRAM GLOBALS
                                           VARS AND STRUCTCS DECLARATION            ***/

#ifndef GLOBALS_C
#define GLOBALS_C

#include "globals.h"

int IC; /* instruction counter */
int DC; /* data counter */
int numRow; /* current file row index */
int numColumn;
int numOfErrors; /* counter for number of errors in each file loop */
int successFiles; /* will count how many files went through the full assembler proccess with no errors */
spLine *pSpLine; /* global splitted line variable */


/* initiate global struct array with all instruction command corresponded to compitable arg type */
 struct instructions ICS[NUM_INSTRUCTION_COMMANDS] /* Array of machine instructions */ = {
	{"mov", {{IMMEDIATE, DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}, {DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"cmp", {{IMMEDIATE, DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}, {IMMEDIATE, DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"add", {{IMMEDIATE, DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}, {DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"sub", {{IMMEDIATE, DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}, {DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"lea", {{DIRECT,  NULL_METHOD}, {DIRECT,  REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"clr", {{NULL_METHOD}, {DIRECT,  REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"not", {{NULL_METHOD}, {DIRECT,  REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"inc", {{NULL_METHOD}, {DIRECT,  REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"dec", {{NULL_METHOD}, {DIRECT,  REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"jmp", {{NULL_METHOD}, {DIRECT, REF_REG, NULL_METHOD}}},
	{"bne", {{NULL_METHOD}, {DIRECT, REF_REG, NULL_METHOD}}},
	{"red", {{NULL_METHOD}, {DIRECT,  REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"prn", {{NULL_METHOD}, {IMMEDIATE, DIRECT, DIRECT_REG, NULL_METHOD}}},
	{"jsr", {{NULL_METHOD}, {DIRECT, REF_REG, DIRECT_REG, NULL_METHOD}}},
	{"rts", {{NULL_METHOD}, {NULL_METHOD}}},
	{"stop", {{NULL_METHOD}, {NULL_METHOD}}}};

#endif