#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "proccesor.h"
#include "spu.h"

int main(void) {
    swag_t swag0 = {};
    swagElem_t* BCbuffer = NULL;
    swagElem_t
    fileInfo byteCodeFile = {"byteCode.txt", 0, 0, NULL, NULL};

    spu_t SPU = {swag0, BCbuffer, 0, NULL};
    BCFileToArr(&byteCodeFile, &SPU.ByteCodeBuf);

    //for (int i = 0; i < 106; i++) {
    //    printf("%d-th elem: %zu\n", i, SPU.ByteCodeBuf[i]);
    //}

    Proccesing(&SPU);

    //swagElem_t valu = 0;
    //SwagPop(&SPU.spu_Swag, &valu);
    //printf("Last in stack: %zu\n", valu);
    //printf("\n");

    return 0;
}

