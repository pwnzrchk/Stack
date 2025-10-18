#include "translator.h"

int main (void) {
    fileInfo mainFile = {"src/ASSCode.asm", 0, 0, NULL, NULL};
    fileInfo byteCodeFile = {"src/byteCode.txt", 0, 0, NULL, NULL};
    Translator Main_Translator = {};

    TranslatorConstructor(&Main_Translator, &mainFile, &byteCodeFile);
    ByteCoder(&Main_Translator);
    printf("Assembled succesfully\n");
    return 0;
}
