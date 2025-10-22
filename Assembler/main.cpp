#include "translator.h"

int main (void) {
    fileInfo mainFile = {"src/ASSCode.asm", 0, 0, NULL, NULL};
    fileInfo byteCodeFile = {"src/byteCode.txt", 0, 0, NULL, NULL};
    Translator Main_Translator = {};

    TranslatorConstructor(&Main_Translator, &mainFile, &byteCodeFile);
    transErr_t byte_code_error = NO_TRANS_ERR;
    if ((byte_code_error = ByteCoder(&Main_Translator)) != NO_TRANS_ERR) {
        ErrorHandler(byte_code_error);
        return kInvalidReturnValue;
    }
    printf("Assembled succesfully\n");
    return 0;
}
