#ifndef proccesor_h
#define proccesor_h

#include <stdio.h>
#include <stdbool.h>

#include "swags.h"
#include "fileFunc.h"
#include "spu.h"

enum ProcErr_t {
    WITHOUT_ERRS = 0,
    NULL_PTR_ERR = 1
};

enum ProcCom_t {
    PUSH = 1,
    POP = 2,
    SUM = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    DUMP = 7,
    HLT = 8
};


ProcErr_t Proccesing(spu_t* refSpu);
fileFunErr_t BCFileToArr(fileInfo* refBCFile, swagElem_t** refArr);

#endif //proccesor_h
