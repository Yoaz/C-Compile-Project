                    /*** THIS IS THE GLOBALS FILE, HERE WE WILL HOLD ALL PROGRAM GLOBALS
                                           VARS AND STRUCTCS DECLARATION            ***/

#include "data.h"

int IC;
int DC;
int numLine;
int numColumn;
int numErrors;


struct insLine{
    char *raw;
    char *
}


struct dirLine{
    char *raw;

}

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
