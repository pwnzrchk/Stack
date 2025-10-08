#include "proccesor.h"


ProcErr_t Proccesing(spu_t* refSpu) {
    if (!refSpu -> ByteCodeBuf) return NULL_PTR_ERR;


    bool FlagOfExit = false;
    bool* FlagOfExitPTR = &FlagOfExit;
    swagErr_t error = NO_ERRS;


    if ((error = SwagInit(&(refSpu -> spu_Swag), 100)) != NO_ERRS) {
        SwagDump(&(refSpu -> spu_Swag), error);
        fprintf(stderr, "SwagInit failed in proc\n");
        return NULL_PTR_ERR;
    }


    for(size_t j = 0; j < 105 && !FlagOfExit;) {    //как ограничить количество
        const swagElem_t comand = refSpu -> ByteCodeBuf[j++];
        swagElem_t arg = 0;
        if (!comand) {
            fprintf(stderr, "Null comand pointer at %zu\n", j);
            break;
        }

        switch(comand) {
            case PUSH:
                if ((error = SwagPush(&(refSpu -> spu_Swag), refSpu -> ByteCodeBuf[j++])) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "PUSH error in proccesing\n");
                    break;
                }
                //printf("PUSHED %zu\n", refSpu -> ByteCodeBuf[j - 1]); //FIXME
                break;

            case POP:
                if ((error = SwagPop(&(refSpu -> spu_Swag), &arg)) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "POP error in proccesing\n");
                    break;
                }
                break;

            case SUM:
                if ((error = SwagSumn(&(refSpu -> spu_Swag))) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "SUM error in proccesing\n");
                    break;
                }
                break;

            case SUB:
                if ((error = SwagSub(&(refSpu -> spu_Swag))) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "SUB error in proccesing\n");
                    break;
                }
                break;

            case MUL:
                if ((error = SwagMul(&(refSpu -> spu_Swag))) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "MUL error in proccesing\n");
                    break;
                }
                break;

            case DIV:
                if ((error = SwagDiv(&(refSpu -> spu_Swag))) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "DIV error in proccesing\n");
                    break;
                }
                break;

            case DUMP:
                if ((error = SwagVerify(&(refSpu -> spu_Swag))) != NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), error);
                    fprintf(stderr, "DUMP error in proccesing string No%zu\n", j + 1);
                } else if (error == NO_ERRS) {
                    SwagDump(&(refSpu -> spu_Swag), NO_ERRS);
                } else {
                    fprintf(stderr, "DUMP unexpected argument at line %zu\n", j + 1);
                }
                break;

            case HLT:
                *FlagOfExitPTR = true;
                break;

            default:
                fprintf(stderr, "Unknown command %zd at line %zu\n", comand, j + 1);
                break;

        }   //SWITCH
    }   //FOR
    //("Exiting...\n");
    return WITHOUT_ERRS;
}



fileFunErr_t BCFileToArr(fileInfo* refBCFile, swagElem_t** refArr) {
    Plenumation(refBCFile);
    Distributor(refBCFile);

    *refArr = (swagElem_t*)calloc((size_t)((refBCFile -> str_count)*2 + 1), sizeof(**refArr));

    if (*refArr == NULL) {
        fprintf(stderr, "Calloc error for BC_buffer\n");
        return NULL_PTR_PLUM;
    }
    if (*refArr != NULL) {
        (*refArr)[(refBCFile -> str_count)*2] = 73552812;
    }

    size_t arrIndex = 0;
    for(size_t j = 0; j < refBCFile -> str_count - 1 && arrIndex < (refBCFile -> str_count)*2; j++) {

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
            (*refArr)[arrIndex++] = comand;
        } else if (scanned == 2) {
            (*refArr)[arrIndex++] = comand;
            (*refArr)[arrIndex++] = arg;
        } else {
            fprintf(stderr, "Wrong line No%zu\n", j+1);
            break;
        }
    }
    return NO_PLUM_ERR;
}















//ProcErr_t Proccesing(fileInfo* BCFile, swag_t* refSwag) {
//    if (!BCFile || !refSwag) return NULL_PTR_ERR;
//
//    Plenumation(BCFile);        //FIXME буффер чаров              Я бувально распихиваю в массив указателей
//    Distributor(BCFile);        //FIXME как работать без чаров?   на чар, и выделяю буффер в чарах
//
//    bool FlagOfExit = false;
//    bool* FlagOfExitPTR = &FlagOfExit;
//    swagErr_t error = NO_ERRS;
//
//    if ((error = SwagInit(refSwag, 66)) != NO_ERRS) {
//        SwagDump(refSwag, error);
//        fprintf(stderr, "SwagInit failed in proc\n");
//        return NULL_PTR_ERR;
//    }
//
//
//
//    for(size_t j = 0; j < BCFile -> str_count - 1 && !FlagOfExit; j++) {
//        const char* workline = BCFile -> pointerBuffer[j];
//        if (!workline) {
//            fprintf(stderr, "Null line pointer at %zu\n", j);
//            break;
//        }
//
//        swagElem_t arg = 0;
//        unsigned comand = 0;
//        short scanned = 0;
//
//        if ((scanned = (short)sscanf(workline, "%u", &comand)) <= 0) {
//            fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
//            continue;
//        }
//
//        switch(comand) {
//            case PUSH: //PUSH
//                if ((scanned = (short)sscanf(workline, "%u %zu", &comand, &arg)) <= 0) {
//                    fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
//                    continue;
//                }
//                if (scanned == 2) {
//                    if ((error = SwagPush(refSwag, arg)) != NO_ERRS) {
//                        SwagDump(refSwag, error);
//                        fprintf(stderr, "PUSH error in proccesing\n");
//                    }
//                } else {
//                    fprintf(stderr, "Reading arg from BYTE Code error in string No%zu\n", j + 1);
//                }
//                break;
//
//            case POP: //POP
//                if (scanned == 1) {
//                    if ((error = SwagPop(refSwag, &arg)) != NO_ERRS) {
//                        fprintf(stderr, "Pop error in proccesing string No%zu\n", j + 1);
//                    }
//                } else {
//                    fprintf(stderr, "POP Scan error in ByteCode, string No:%zu\n", j + 1);
//                }
//                break;
//
//            case SUM: //SUM
//                if (scanned == 1) {
//                    if ((error = SwagSumn(refSwag)) != NO_ERRS) {
//                        fprintf(stderr, "Sum error in proccesing string No%zu\n", j + 1);
//                    }
//                } else  {
//                    fprintf(stderr, "SUM Scan error in ByteCode, string No:%zu\n", j + 1);
//                }
//                break;
//
//            case SUB: //SUB
//                if (scanned == 1) {
//                    if ((error = SwagSub(refSwag)) != NO_ERRS) {
//                        fprintf(stderr, "Sub error in proccesing string No%zu\n", j + 1);
//                    }
//                } else {
//                    fprintf(stderr, "SUB Scan error in ByteCode, string No:%zu\n", j + 1);
//                }
//                break;
//
//            case MUL:
//                if (scanned == 1) {
//                    if ((error = SwagMul(refSwag)) != NO_ERRS) {
//                        fprintf(stderr, "Mul error in proccesing string No%zu\n", j + 1);
//                    }
//                } else {
//                    fprintf(stderr, "MUL Scan error in ByteCode, string No:%zu\n", j + 1);
//                }
//                break;
//
//            case DIV: //Derivate
//                if (scanned == 1) {
//                    if ((error = SwagDiv(refSwag)) != NO_ERRS) {
//                        fprintf(stderr, "Div error in proccesing string No%zu\n", j + 1);
//                    }
//                } else {
//                    fprintf(stderr, "DIV Scan error in ByteCode, string No:%zu\n", j + 1);
//                }
//                break;
//
//            case DUMP:
//                if (scanned == 1) {
//                    if ((error = SwagVerify(refSwag)) != NO_ERRS) {
//                        SwagDump(refSwag, error);
//                        fprintf(stderr, "DUMP error in proccesing string No%zu\n", j + 1);
//                    } else {
//                        SwagDump(refSwag, NO_ERRS);
//                    }
//                } else {
//                    fprintf(stderr, "DUMP unexpected argument at line %zu\n", j + 1);
//                }
//                break;
//
//            case HLT: //HLT
//                if (scanned == 1) {
//                    *FlagOfExitPTR = true;
//                    break;
//                } else {
//                    fprintf(stderr, "HULT unexpected argument at line %zu\n", j + 1);
//                }
//                break;
//
//            default:
//                fprintf(stderr, "Unknown command %u at line %zu\n", comand, j + 1);
//                break;
//
//        }   //SWITCH
//    }   //FOR
//    printf("Exiting...\n");
//    return WITHOUT_ERRS;
//}
