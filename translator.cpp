#include "translator.h"


transErr_t ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf) {
    if (!refFileInf || !byteCodeFileInf) return NULL_PTR_TRANSL;
    char arrForWord[99];
    int argument = 0;

    FILE* fileByteCode = fopen(byteCodeFileInf -> file_name, "w");
    if (!fileByteCode) {
        fprintf(stderr, "File open error - byteCode\n");
        return OPEN_FILE_ERR;
    }
    byteCodeFileInf -> str_count = refFileInf -> str_count;


    for (size_t k = 0; k < refFileInf -> str_count - 1; k++) {
        const char* workline = NULL;
        if (!refFileInf -> pointerBuffer) break;
        workline = refFileInf -> pointerBuffer[k];
        if (!workline) break;

        short scanned = 0;
            if ((scanned = (short)sscanf(workline, "%98s %d", arrForWord, &argument)) <= 0) {
                fprintf(stderr, "Error scanf ASM file\nString No %zu\n", k);
                break;
            }


            if (strncmp(arrForWord, "PUSH", 4) == 0) {
                if (scanned == 2) {
                    fprintf(fileByteCode, "1 %d\n", argument);
            } else {
                    fprintf(stderr, "No argument for PUSH in string No%zu\n", k + 1);
                    break; //FIXME бля что делать с ошибками
            }
            } else if (strncmp(arrForWord, "POP", strlen(workline)) == 0) {
                fprintf(fileByteCode, "2\n");
            } else if (strncmp(arrForWord, "SUM", strlen(workline)) == 0) {
                fprintf(fileByteCode, "3\n");
            } else if (strncmp(arrForWord, "SUB", strlen(workline)) == 0) {
                fprintf(fileByteCode, "4\n");
            } else if (strncmp(arrForWord, "MUL", strlen(workline)) == 0) {
                fprintf(fileByteCode, "5\n");
            } else if (strncmp(arrForWord, "DIV", strlen(workline)) == 0) {
                fprintf(fileByteCode, "6\n");
            } else if (strncmp(arrForWord, "DUMP", strlen(workline)) == 0) {
                fprintf(fileByteCode, "7\n");
            } else if (strncmp(arrForWord, "HLT", strlen(workline)) == 0) {
                fprintf(fileByteCode, "8\n");
            } else {
                fprintf(fileByteCode, "Error in ASM code in string No %zu: \"%s\"\n", k + 1, arrForWord);
                fprintf(stderr, "Error in ASM code in string No %zu: \"%s\"\n", k + 1, arrForWord);
                break;
            }
        }
        fclose(fileByteCode);
    return NO_TRANS_ERR;
}


