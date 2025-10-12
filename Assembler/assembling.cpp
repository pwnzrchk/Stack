#include "translator.h"

int main (void) {
    fileInfo mainFile = {"src/ASSCode.asm", 0, 0, NULL, NULL};
    fileInfo byteCodeFile = {"src/byteCode.txt", 0, 0, NULL, NULL};
    Plenumation(&mainFile);
    Distributor(&mainFile);
    ByteCoder(&mainFile, &byteCodeFile);
    printf("Assembled succesfully\n");
    return 0;
}
