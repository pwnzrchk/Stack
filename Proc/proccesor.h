#ifndef proccesor_h
#define proccesor_h

#include <stdio.h>
#include <stdbool.h>


#include "../Common/fileFunc.h"
#include "../Common/countSymb.h"
#include "swags.h"

const int InitialSize = 55;
const int REG_SIZE = 4;

typedef struct {
    swag_t Swag;
    swagElem_t* ByteCodeBuf;
    size_t pc;
    swagElem_t regs[REG_SIZE];
}spu_t;


enum ProcErr_t {
    WITHOUT_ERRS = 0,
    UNKNW_CMD = 100,
    NULL_PTR_ERR = 1,
    REG_POP_ERR = 2,
    REG_PUSH_ERR = 3,
    PUSH_ERR = 4,
    POP_ERR = 5,
    SUM_ERR = 6,
    SUB_ERR = 7,
    MUL_ERR = 8,
    DIV_ERR = 9,
    DUMP_ERR = 10,
    PUSHR_ERR = 11,
    POPR_ERR = 12

};

enum ProcCom_t {
    PUSH = 1,
    POP = 2,
    SUM = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    DUMP = 7,
    HLT = 8,
    PUSHR = 9,
    POPR = 10
};


ProcErr_t Proccesing(spu_t* refSpu);
fileFunErr_t BCFileToArr(fileInfo* refBCFile, swagElem_t** refArr);
ProcErr_t SpuConstructor(spu_t* refSpu, fileInfo* refBCFile);
ProcErr_t RegPop (spu_t* refSpu, size_t i);
ProcErr_t RegPush(spu_t* refSpu, size_t i);

#endif //proccesor_h
