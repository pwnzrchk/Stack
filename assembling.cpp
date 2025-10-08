#include "translator.h"
#include "fileFunc.h"

int main (void) {
    fileInfo mainFile = {"ASSCode.asm", 0, 0, NULL};
    fileInfo byteCodeFile = {"byteCode.txt", 0, 0, NULL};
    Plenumation(&mainFile);
    Distributor(&mainFile);
    ByteCoder(&mainFile, &byteCodeFile);
    return 0;
}
