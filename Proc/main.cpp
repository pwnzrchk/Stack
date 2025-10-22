#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "proccesor.h"


int main(void) {
    fileInfo byteCodeFile = {"src/byteCode.txt", 0, 0, NULL, NULL};
    spu_t main_spu = {};

    SpuConstructor(&main_spu, &byteCodeFile);
    ProcErr_t code_error = WITHOUT_ERRS;
    if ((code_error = Proccesing(&main_spu)) != WITHOUT_ERRS) {
        ErrorHandler(code_error);
        return 0;
    }
    printf("Processed well...\n");
    return 0;
}
