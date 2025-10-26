#ifndef proccesor_h
#define proccesor_h


#include <stdio.h>
#include <stdbool.h>

#include "../Common/fileFunc.h"
#include "../Common/countSymb.h"
#include "../Common/swags.h"
#include "../Common/commands.h"

//=================================================================================================================================================

const int kInitialSize = 55;
const int kSizeOfRam = 100;

typedef struct {
    swag_t      Swag;
    int*        ByteCodeBuf;
    int         size_of_bytecode_buffer;
    size_t      pc;
    swagElem_t  regs[kRegistersAmount];
    swag_t      stack_return_addresses;
    swagElem_t* ram;
}spu_t;

enum ProcErr_t {
    WITHOUT_ERRS       = 0,
    NULL_PTR_ERR       = 1,
    REG_POP_ERR        = 2,
    REG_PUSH_ERR       = 3,
    PUSH_ERR           = 4,
    POP_ERR            = 5,
    Sumn_ERR           = 6,
    Sub_ERR            = 7,
    Mul_ERR            = 8,
    Div_ERR            = 9,
    DUMP_ERR           = 10,
    JMP_ERR            = 13,
    kRetError          = 14,
    kCallError         = 15,
    kPushMemoryError   = 16,
    kPopMemoryError    = 17,
    kSpuDestroyError   = 18,
    kErrorSize         = 19,
    UNKNW_CMD          = 99,
    INCORECT_SIZE      = 100
};

//=================================================================================================================================================
//General functions
ProcErr_t Proccesing      (spu_t* refSpu);
fileFunErr_t BCFileToArr  (fileInfo* refBCFile, swagElem_t** refArr);
ProcErr_t SpuConstructor  (spu_t* refSpu, fileInfo* refBCFile);
ProcErr_t MemoryCalculator(fileInfo* reference_byte_code_file, size_t* calculated_size);
ProcErr_t SpuDestroyer    (spu_t* spu);
ProcErr_t DrawMemory      (spu_t* spu);

//Variations of JMP's
ProcErr_t Jmp(spu_t* refSpu, size_t index);
ProcErr_t Jbe(spu_t* refSpu, size_t index);
ProcErr_t Jb (spu_t* refSpu, size_t index);
ProcErr_t Jae(spu_t* refSpu, size_t index);
ProcErr_t Ja (spu_t* refSpu, size_t index);
ProcErr_t Je (spu_t* refSpu, size_t index);
ProcErr_t Jne(spu_t* refSpu, size_t index);

//Functions for work with registers
ProcErr_t RegPop (spu_t* refSpu, size_t i);
ProcErr_t RegPush(spu_t* refSpu, size_t i);

//Call and return functions
ProcErr_t CallFunction(spu_t* spu, size_t new_pc);
ProcErr_t RetFunction (spu_t* spu);

//Functions for work with RAM
ProcErr_t PopMemory (spu_t* spu, swagElem_t register_number);
ProcErr_t PushMemory(spu_t* spu, swagElem_t register_number);

void ErrorHandler(int error_code);

#endif //proccesor_h
