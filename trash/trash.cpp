
FIX - Return error and print this message in main
FIX - #define with args and using of ##







tepedef struct {
    swag_t

}Labe_info;




//ProcErr_t Proccesor(spu_t* refSpu) {
//
//}






//#define PENIS(file, p2) fprintf(file, p2);
//
//PENIS(stdin, "BOBA");


//ProcErr_t Proccesing(fileInfo* BCFile, swag_t* refSwag) {
//    if (!BCFile || !refSwag) return NULL_PTR_ERR;
//
//    Plenumation(BCFile);        //FIX буффер чаров              Я бувально распихиваю в массив указателей
//    Distributor(BCFile);        //FIX как работать без чаров?   на чар, и выделяю буффер в чарах
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
