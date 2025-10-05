#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "swags.h"
#include "menu.h"
#include "translator.h"
#include "proccesor.h"

int main(void) {

    swag_t swag0 = {};
    fileInfo mainFile = {"ASSCode.asm", 0, 0, NULL};
    fileInfo byteCodeFile = {"byteCode.txt", 0, 0, NULL};
    Plenumation(&mainFile);
    Distributor(&mainFile);
    ByteCoder(&mainFile, &byteCodeFile);
    Proccesing(&byteCodeFile, &swag0);
    swagElem_t argum = 183;
    printf("Last: %d\n", SwagPop(&swag0, &argum));
    return 0;
}

