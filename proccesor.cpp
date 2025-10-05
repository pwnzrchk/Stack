#include <stdio.h>
#include <stdbool.h>

#include "swags.h"
#include "translator.h"

enum ProcErr_t {
    WITHOUTERRS = 0,
    NULLPTRERR = 1
};


ProcErr_t Proccesing(fileInfo* BCFile, swag_t* refSwag) {
    if (!BCFile || !refSwag) return NULLPTRERR;

    Plenumation(BCFile);        //FIXME буффер чаров              Я бувально распихиваю в массив указателей
    Distributor(BCFile);        //FIXME как работать без чаров?   на чар, и выделяю буффер в чарах

    bool FlagOfExit = false;
    bool* FlagOfExitPTR = &FlagOfExit;
    swagErr_t error = NOERRS;

    if ((error = SwagInit(refSwag, 66)) != NOERRS) {
        SwagDump(refSwag, error);
        fprintf(stderr, "SwagInit failed in proc\n");
        return NULLPTRERR;
    }



    for(size_t j = 0; j < BCFile -> str_count - 1 && !FlagOfExit; j++) {
        const char* workline = BCFile -> pointerBuffer[j];
        if (!workline) {
            fprintf(stderr, "Null line pointer at %zu\n", j);
            break;
        }

        swagElem_t arg = 0;
        unsigned comand = 0;
        short scanned = 0;

        if ((scanned = (short)sscanf(workline, "%u", &comand)) <= 0) {
            fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
            continue;
        }

        switch(comand) {
            case 1: //PUSH
                if ((scanned = (short)sscanf(workline, "%u %d", &comand, &arg)) <= 0) {
                    fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
                    continue;
                }
                if (scanned == 2) {
                    if ((error = SwagPush(refSwag, arg)) != NOERRS) {
                        SwagDump(refSwag, error);
                        fprintf(stderr, "PUSH error in proccesing\n");
                    }
                } else {
                    fprintf(stderr, "Reading arg from BYTE Code error in string No%zu\n", j + 1);
                }
                break;

            case 2: //POP
                if (scanned == 1) {
                    if ((error = SwagPop(refSwag, &arg)) != NOERRS) {
                        fprintf(stderr, "Pop error in proccesing string No%zu\n", j + 1);
                    }
                } else {
                    fprintf(stderr, "POP Scan error in ByteCode, string No:%zu\n", j + 1);
                }
                break;

            case 3: //SUM
                if (scanned == 1) {
                    if ((error = SwagSumn(refSwag)) != NOERRS) {
                        fprintf(stderr, "Sum error in proccesing string No%zu\n", j + 1);
                    }
                } else  {
                    fprintf(stderr, "SUM Scan error in ByteCode, string No:%zu\n", j + 1);
                }
                break;

            case 4: //SUB
                if (scanned == 1) {
                    if ((error = SwagSub(refSwag)) != NOERRS) {
                        fprintf(stderr, "Sub error in proccesing string No%zu\n", j + 1);
                    }
                } else {
                    fprintf(stderr, "SUB Scan error in ByteCode, string No:%zu\n", j + 1);
                }
                break;

            case 5:
                if (scanned == 1) {
                    if ((error = SwagMul(refSwag)) != NOERRS) {
                        fprintf(stderr, "Mul error in proccesing string No%zu\n", j + 1);
                    }
                } else {
                    fprintf(stderr, "MUL Scan error in ByteCode, string No:%zu\n", j + 1);
                }
                break;

            case 6: //Derivate
                if (scanned == 1) {
                    if ((error = SwagDiv(refSwag)) != NOERRS) {
                        fprintf(stderr, "Div error in proccesing string No%zu\n", j + 1);
                    }
                } else {
                    fprintf(stderr, "DIV Scan error in ByteCode, string No:%zu\n", j + 1);
                }
                break;

            case 7:
                if (scanned == 1) {
                    if ((error = SwagVerify(refSwag)) != NOERRS) {
                        SwagDump(refSwag, error);
                        fprintf(stderr, "DUMP error in proccesing string No%zu\n", j + 1);
                    } else {
                        SwagDump(refSwag, NOERRS);
                    }
                } else {
                    fprintf(stderr, "DUMP unexpected argument at line %zu\n", j + 1);
                }
                break;

            case 8: //HLT
                if (scanned == 1) {
                    *FlagOfExitPTR = true;
                    break;
                } else {
                    fprintf(stderr, "HULT unexpected argument at line %zu\n", j + 1);
                }
                break;

            default:
                fprintf(stderr, "Unknown command %u at line %zu\n", comand, j + 1);
                break;

        }   //SWITCH
    }   //FOR
    printf("Exiting...\n");
    return WITHOUTERRS;
}






