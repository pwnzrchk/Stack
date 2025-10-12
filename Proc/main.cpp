#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "proccesor.h"


int main(void) {

    fileInfo byteCodeFile = {"src/byteCode.txt", 0, 0, NULL, NULL};
    spu_t mainSPU = {};
    SpuConstructor(&mainSPU, &byteCodeFile);

    //printf("pc in refSpu %d\n", mainSPU.pc);
    // for (int i = 0; i < (byteCodeFile.str_count) *2 + 2; i++) {
        // printf("%d elem is: %10zu\n ", i, mainSPU.ByteCodeBuf[i]);
    // }
    // printf("Size of BCArr arr and comands amount: %zu\n", mainSPU.ByteCodeBuf[0]);
    ProcErr_t ProcErr = WITHOUT_ERRS;
    if ((ProcErr = Proccesing(&mainSPU)) != WITHOUT_ERRS) {
        printf("ProcErr_t error №%d\n", ProcErr);
    }
    //функция выводящая строку исходя из кода ошибки
    printf("Processed well...\n");
    return 0;
}
