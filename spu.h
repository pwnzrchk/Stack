#ifndef spu_h
#define spu_h

#include "fileFunc.h"
#include "proccesor.h"

typedef struct {
    swag_t spu_Swag;
    swagElem_t* ByteCodeBuf;
    size_t operations;
    swagElem_t regs[8];
}spu_t;

#endif  //spu_h
