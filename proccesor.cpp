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
    Distributor(BCFile);        //FIXME как работать без чаров?   на чар, и выделяю буфферв чарах

    unsigned comand = 0;
    swagElem_t arg = 0;
    bool FlagOfExit = false;
    bool* FlagOfExitPTR = &FlagOfExit;
    swagErr_t error = NOERRS;
    SwagInit(refSwag, 66);

    for(size_t j = 0; j < BCFile -> str_count - 1 && FlagOfExit == false; j++) {
        const char* workline = BCFile -> pointerBuffer[j];
        if (!workline) break;   //обработка ошибок

        short scanned = 0;
        if ((scanned = (short)sscanf(workline, "%u %d", &comand, &arg)) <= 0) {
            fprintf(stderr, "Error scanf BYTECODE file\nString No %zu\n", j + 1);
            break;
        }

        switch(comand) {
            case 1:
                if (scanned == 2) {
                    if ((error = SwagPush(refSwag, arg)) != NOERRS) {
                        SwagDump(refSwag, error);
                        fprintf(stderr, "PUSH error in proccesing\n");
                        break;
                    }
                } else {
                    fprintf(stderr, "Reading arg from BYTE Code error in string No%zu", j + 1);
                    break;
                }

            case 2:
                if (scanned == 1) {
                    if ((error = SwagPop(refSwag, &arg)) != NOERRS) {
                        fprintf(stderr, "Pop error in proccesing string No%zu\n", j + 1);
                        break;
                    }
                } else {
                    fprintf(stderr, "POP Scan error in ByteCode, string No:%zu\n", j + 1);
                    break;
                }

            case 3:
                if (scanned == 1) {
                    if ((error = SwagSumn(refSwag)) != NOERRS) {
                        fprintf(stderr, "Sum error in proccesing string No%zu\n", j + 1);
                        break;
                    }
                } else {
                    fprintf(stderr, "SUM Scan error in ByteCode, string No:%zu\n", j + 1);
                    break;
                }

            case 4:
                if (scanned == 1) {
                    if ((error = SwagSub(refSwag)) != NOERRS) {
                        fprintf(stderr, "Sub error in proccesing string No%zu\n", j + 1);
                        break;
                    }
                } else {
                    fprintf(stderr, "SUB Scan error in ByteCode, string No:%zu\n", j + 1);
                    break;
                }

            case 5:
                if (scanned == 1) {
                    if ((error = SwagMul(refSwag)) != NOERRS) {
                        fprintf(stderr, "Mul error in proccesing string No%zu\n", j + 1);
                        break;
                    }
                } else {
                    fprintf(stderr, "MUL Scan error in ByteCode, string No:%zu\n", j + 1);
                    break;
                }

            case 6:
                if (scanned == 1) {
                    if ((error = SwagDiv(refSwag)) != NOERRS) {
                        fprintf(stderr, "Div error in proccesing string No%zu\n", j + 1);
                        break;
                    }
                } else {
                    fprintf(stderr, "DIV Scan error in ByteCode, string No:%zu\n", j + 1);
                    break;
                }

            case 7:
                if (scanned == 1) {
                    if ((error = SwagVerify(refSwag)) != NOERRS) {
                        SwagDump(refSwag, error);
                        fprintf(stderr, "Verifying error in proccesing string No%zu\n", j + 1);
                        break;
                    }
                } else {
                    fprintf(stderr, "VERIFY Scan error in ByteCode, string No:%zu\n", j + 1);
                    break;
                }

            case 8:
                if (scanned == 1) {
                    *FlagOfExitPTR = true;
                    break;
                }

            default:
                fprintf(stderr, "There is no command in ByteCode\n");

        }
    }
    printf("Exiting...\n");
    return WITHOUTERRS;
}






