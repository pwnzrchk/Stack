#include "proccesor.h"

#define SwagAdr (refSpu -> Swag)     //почему они бесполезны?
#define MakeErr(code)  code##_ERR   //ПОЧЕМУ?
#define REPLACE(command) (error = Swag##command(&(SwagAdr))) != NO_ERRS

//=================================================================================================================================================

#define CASE_TEMPLATE_ERROR_HANDLER (error_code)                \
        fprintf(stderr, #error_code " error in processing\n");  \
        break;

//=================================================================================================================================================

#define REPLACEMENT(comand)                                         \
        if (REPLACE(comand)) {                                      \
            SwagDump(&(SwagAdr), error);                            \
            fprintf(stderr, #comand " error in processing\n");      \
            return MakeErr(comand);                                 \
        }                                                           \
        break;

//=================================================================================================================================================

#define JMP_TEMPLATE(operation, printCode)                                                                  \
        if (index < 1 || index >= refSpu->ByteCodeBuf[0]) {                                                 \
            return INCORECT_SIZE;                                                                           \
        }                                                                                                   \
        swagElem_t compared1, compared2 = 0;                                                                \
        if ((SwagPop(&SwagAdr, &compared1) != NO_ERRS) || (SwagPop(&SwagAdr, &compared2) != NO_ERRS)) {     \                                                        \
            return JMP_ERR;                                                                                 \
        }                                                                                                   \
        if (compared1 operation compared2) refSpu->pc = index + 1;                                          \
        return WITHOUT_ERRS;

//=================================================================================================================================================

#define JMP_TEMP_CASE(jmp_var)                                                                              \
        if ((Proc_err = jmp_var(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {        \
            return JMP_ERR;                                                                                 \
        }                                                                                                   \
        break;

//=================================================================================================================================================



ProcErr_t Proccesing(spu_t* refSpu) {
    if (refSpu -> ByteCodeBuf == NULL)  return  NULL_PTR_ERR;
    if ((refSpu -> regs) == NULL)       return  NULL_PTR_ERR;
    if(refSpu -> Swag.data == NULL)     return  NULL_PTR_ERR;

    bool FlagOfExit = false;
    swagErr_t error = NO_ERRS;
    ProcErr_t Proc_err = WITHOUT_ERRS;

    swagElem_t BC_Buffer_Size = refSpu -> ByteCodeBuf[0];
    for(refSpu -> pc = 0; refSpu -> pc < BC_Buffer_Size && !FlagOfExit;) {
        const swagElem_t comand = refSpu -> ByteCodeBuf[(++refSpu ->pc)];
        swagElem_t arg = 0;
        if (!comand) {
            fprintf(stderr, "Null comand pointer in %zu ell in ARR\n", refSpu -> ByteCodeBuf[refSpu ->pc] + 1);
            return NULL_PTR_ERR;
        }
// ErrorHandler - функция для обработки ошибок (принимает номер ошибки и выводит сообщение)
        switch(comand) {
            case PUSH:
                if ((error = SwagPush(&(SwagAdr), refSpu -> ByteCodeBuf[++(refSpu ->pc)])) != NO_ERRS) {
                    SwagDump(&(SwagAdr), error);
                    return PUSH_ERR;
                }
                break;

            case POP:
                if ((error = SwagPop(&(refSpu -> Swag), &arg)) != NO_ERRS) {
                    SwagDump(&(refSpu -> Swag), error);
                    return POP_ERR;
                }
                break;

            case SUM:
                REPLACEMENT(Sumn);

            case SUB:
                REPLACEMENT(Sub);

            case MUL:
                REPLACEMENT(Mul);

            case DIV:
                REPLACEMENT(Div);
            //REVIEW - same, its clearlier isnt it?
            case DUMP:
                if (REPLACE(Verify)) {
                    SwagDump(&(SwagAdr), error);
                    return DUMP_ERR;
                } else if (error == NO_ERRS) {
                    SwagDump(&(SwagAdr), NO_ERRS);
                }
                break;

            case HLT:
                FlagOfExit = true;
                break;

            case PUSHR:
                if ((Proc_err = RegPush(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {
                    return Proc_err;
                }
                break;

            case POPR:
                if ((Proc_err = RegPop(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {
                    return Proc_err;
                }
                break;

            case JMP:   JMP_TEMP_CASE(Jmp);

            case JB:    JMP_TEMP_CASE(Jb);

            case JBE:   JMP_TEMP_CASE(Jbe);

            case JA:    JMP_TEMP_CASE(Ja);

            case JAE:   JMP_TEMP_CASE(Jae);

            case JE:    JMP_TEMP_CASE(Je);

            case JNE:   JMP_TEMP_CASE(Jne);

            case ERROR_COM:
            default:
                return UNKNW_CMD;

        }
    }
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

fileFunErr_t BCFileToArr(fileInfo* refBCFile, swagElem_t** refArr) {
    assert(refBCFile != NULL && refArr != NULL);
    Plenumation(refBCFile);
    Distributor(refBCFile);

    *refArr = (swagElem_t*)calloc((size_t)((refBCFile -> str_count)*2 + 2), sizeof(**refArr));

    if (*refArr == NULL) {
        fprintf(stderr, "Calloc error for BC_buffer\n");
        return NULL_PTR_PLUM;
    }
    (*refArr)[(refBCFile -> str_count)*2 + 1] = SWAGVIPERR;
    (*refArr)[0] = (refBCFile -> str_count)*2 + 1;

    size_t arrIndex = 0;
    size_t BC_Buffer_Size = (refBCFile -> str_count)*2 + 1;
    size_t Commands_Amount = refBCFile -> str_count - 1;
    for(size_t j = 0; j < Commands_Amount && arrIndex < BC_Buffer_Size + 1; j++) {

        const char* workline = refBCFile -> pointerBuffer[j];
        if (!workline) {
            fprintf(stderr, "Null line pointer at %zu\n", j+1);
            break;
        }

        swagElem_t arg = 0;
        swagElem_t comand = 0;
        int scanned = 0;


        if ((scanned = sscanf(workline, "%zd %zd", &comand, &arg)) <= 0) {
            fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
            break;
        } else if (scanned == 1) {
            (*refArr)[++arrIndex] = comand;
        } else if (scanned == 2) {
            (*refArr)[++arrIndex] = comand;
            (*refArr)[++arrIndex] = arg;
        } else {
            fprintf(stderr, "Wrong line No%zu\n", j+1);
            break;
        }
    }
    return NO_PLUM_ERR;
}

//=================================================================================================================================================

ProcErr_t SpuConstructor(spu_t* refSpu, fileInfo* refBCFile) {
    assert(refBCFile != NULL);
    assert(refSpu != NULL);

    swag_t SpuSwag = {};
    SwagInit(&SpuSwag, InitialSize);
    for (int i = 0; i < kRegistersAmount; i++) {
        refSpu -> regs[i] = 0;
    }

    refSpu -> Swag = SpuSwag;
    BCFileToArr(refBCFile, &(refSpu -> ByteCodeBuf));   //А как штука инициализируется?
    refSpu -> pc = 0;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t RegPush(spu_t* refSpu, size_t i) {
    assert(i < 5 && i > 0);
    assert(refSpu != NULL);

    if (!(i < 5 && i > 0)) return INCORECT_SIZE;

    swagElem_t temp = 0;
    swagErr_t ErrRegPush = SwagPop(&(refSpu -> Swag), &temp);
    if (ErrRegPush != NO_ERRS) {
        return REG_PUSH_ERR;
    }
    refSpu -> regs[i - 1] = temp;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t RegPop (spu_t* refSpu, size_t i) {
    assert(refSpu != NULL);
    if (!(i < 5 && i > 0)) return INCORECT_SIZE;

    swagErr_t ErrPopReg = SwagPush(&(refSpu -> Swag), refSpu -> regs[i - 1]);
    if (ErrPopReg != NO_ERRS) return REG_POP_ERR;

    return WITHOUT_ERRS;

}

//=================================================================================================================================================

ProcErr_t Jmp(spu_t* refSpu, size_t index) {
    assert(refSpu != NULL);

    if (index < 1 || index >= refSpu->ByteCodeBuf[0]) {
        return INCORECT_SIZE;
    }
    refSpu ->pc = index;
    return WITHOUT_ERRS;
}

//=================================================================================================================================================

ProcErr_t Jbe(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(<=, JBE);
}

ProcErr_t Jb(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(<, JB);
}

ProcErr_t Jae(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(>=, JAE);
}

ProcErr_t Ja(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(>, JA);
}

ProcErr_t Je(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(==, JE);
}

ProcErr_t Jne(spu_t* refSpu, size_t index) {
    JMP_TEMPLATE(!=, JNE);
}

//=================================================================================================================================================

void ErrorHandler(int error_code) {
    switch(error_code) {
        case NULL_PTR_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Null_pointer)

        case REG_POP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Pop_register)

        case REG_PUSH_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Push_register)

        case PUSH_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Push)

        case POP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Pop)

        case Sumn_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Sum)

        case Sub_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Sub)

        case Mul_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Mul)

        case Div_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Div)

        case DUMP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Dumping)

        case JMP_ERR:
            CASE_TEMPLATE_ERROR_HANDLER(Jump)

        case INCORECT_SIZE:
            CASE_TEMPLATE_ERROR_HANDLER(Size)

        case UNKNW_CMD:
            CASE_TEMPLATE_ERROR_HANDLER(Comand)

        default:
            fprintf(stderr, "ErrorHandler mistake");
    }
}

//=================================================================================================================================================
#undef SwagAdr
#undef MakeErr
#undef REPLACE
#undef REPLACEMENT
#undef JMP_TEMPLATE
#undef JMP_TEMP_CASE

// ProcErr_t SpuDump(spu_t* refSpu) {
    // printf();
// }
//
