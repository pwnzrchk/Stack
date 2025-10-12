#ifndef translator_h
#define translator_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Common/fileFunc.h"


enum transErr_t {
    NO_TRANS_ERR = 10,
    OPEN_FILE_ERR = 11,
    NULL_PTR_TRANSL = 12,
};

enum AsmCom_t {
    PUSH = 1,
    POP = 2,
    SUM = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    DUMP = 7,
    HLT = 8,
    PUSHR = 9,
    POPR = 10,
    JMP = 11,
    ERROR_COM = 99
};

enum AsmReg_t {
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4,
    ERROR_REG = 98
};

transErr_t ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf);
int funcFinder (char* refLine);
int regFinder (char* refLine);


#endif //translator_h
