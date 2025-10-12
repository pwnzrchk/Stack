#include "proccesor.h"
#define SwagAdr (refSpu -> Swag)
#define REPLACE(comand) ((error = Swag##comand(&(SwagAdr))) != NO_ERRS)

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
        // printf("pc in refSpu %d\n", refSpu->pc);
        swagElem_t arg = 0;
        if (!comand) {
            fprintf(stderr, "Null comand pointer in %zu ell in ARR\n", refSpu -> ByteCodeBuf[refSpu ->pc] + 1);
            break;
        }

        switch(comand) {
            case PUSH:
                if ((error = SwagPush(&(refSpu -> Swag), refSpu -> ByteCodeBuf[++(refSpu ->pc)])) != NO_ERRS) {
                    SwagDump(&(refSpu -> Swag), error);
                    fprintf(stderr, "PUSH error in proccesing\n");
                    return PUSH_ERR;
                }
                break;

            case POP:

                if ((error = SwagPop(&(refSpu -> Swag), &arg)) != NO_ERRS) {
                    SwagDump(&(refSpu -> Swag), error);
                    fprintf(stderr, "POP error in proccesing\n");
                    return POP_ERR;
                }
                break;

            case SUM:

                if (REPLACE(Sumn)) {
                    SwagDump(&(SwagAdr), error);
                    fprintf(stderr, "SUM error in proccesing\n");
                    return SUM_ERR;
                }
                break;
// FIXME - #define with args and using of ##
            case SUB:
                if (REPLACE(Sub)) {
                    SwagDump(&(SwagAdr), error);
                    fprintf(stderr, "SUB error in proccesing\n");
                    return SUB_ERR;
                }
                break;

            case MUL:
                if (REPLACE(Mul)) {
                    SwagDump(&(SwagAdr), error);
                    fprintf(stderr, "MUL error in proccesing\n");
                    return MUL_ERR;
                }
                break;

            case DIV:
                if (REPLACE(Div)) {
                    SwagDump(&(SwagAdr), error);
                    fprintf(stderr, "DIV error in proccesing\n");
                    return DIV_ERR;
                }
                break;

            case DUMP:
                if (REPLACE(Verify)) {
                    SwagDump(&(SwagAdr), error);
                    fprintf(stderr, "DUMP error in proccesing\n");
                    return DUMP_ERR;
                } else if (error == NO_ERRS) {
                    SwagDump(&(SwagAdr), NO_ERRS);
                }
                break;

            case HLT:
                FlagOfExit = true;
                printf("Exit!\n");
                break;

            case PUSHR:
                if ((Proc_err = RegPush(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {
                    fprintf(stderr, "PUSHR error in proccesing\n");
                    return PUSHR_ERR;
                }

                break;

            case POPR:
                if ((Proc_err = RegPop(refSpu, refSpu -> ByteCodeBuf[++(refSpu -> pc)])) != WITHOUT_ERRS) {
                    fprintf(stderr, "POPR error in proccesing\n");
                    return POPR_ERR;
                }
                break;

            default:
            // FIXME - Return error and print this message in main
                fprintf(stderr, "Unknown command %zd in element %zu in ARR\n", comand, refSpu ->pc);
                return UNKNW_CMD;

        }   //SWITCH
    }   //FOR
    //("Exiting...\n");
    return WITHOUT_ERRS;
}



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


ProcErr_t SpuConstructor(spu_t* refSpu, fileInfo* refBCFile) {
    assert(refBCFile != NULL);
    swag_t SpuSwag = {};
    SwagInit(&SpuSwag, InitialSize);
    for (int i = 0; i < 4; i++) {
        refSpu -> regs[i] = 0;
    }
    //refSpu -> regs = memset(refSpu -> regs, 0, 4);
    refSpu -> Swag = SpuSwag;
    BCFileToArr(refBCFile, &(refSpu -> ByteCodeBuf));   //А как штука инициализируется?
    refSpu -> pc = 0;
    return WITHOUT_ERRS;
}





ProcErr_t RegPush(spu_t* refSpu, size_t i) {
    assert(i < 5 && i > 0);
    swagElem_t temp = 0;
    swagErr_t ErrRegPush = SwagPop(&(refSpu -> Swag), &temp);
    if (ErrRegPush != NO_ERRS) {
        fprintf(stderr, "Error pushing register\n");
        return REG_PUSH_ERR;
    }
    refSpu -> regs[i - 1] = temp;
    return WITHOUT_ERRS;
}

ProcErr_t RegPop (spu_t* refSpu, size_t i) {
    assert(i < 5 && i > 0);
    swagErr_t ErrPopReg = SwagPush(&(refSpu -> Swag), refSpu -> regs[i - 1]);
    if (ErrPopReg != NO_ERRS) {
        return REG_POP_ERR;
    } else {
        return WITHOUT_ERRS;
    }
}
