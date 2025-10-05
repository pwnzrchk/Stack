#include <stdio.h>
#include <sys/stat.h>

#include "countSymb.H"
#include "swags.h"



enum transErr_t {
    NOTRANSERR = 10,
    OPENFILEERR = 11,
    NULLTRANS = 12,
    FREADERR = 13,
    CALLOCERR = 14,
    SIZEERR = 15
};

typedef struct{
    const char* file_name;
    size_t str_count;
    long long file_size;
    char* buffer;
    char** pointerBuffer;
 }fileInfo;


long long FileSize(FILE* refFile) {
    struct stat fileFeature;
    fstat(fileno(refFile), &fileFeature);
    return fileFeature.st_size;
}



transErr_t Plenumation(fileInfo* refFileArch) {
    FILE* file = fopen(refFileArch -> file_name, "r");
    if (!file) {
        fprintf(stderr, "Open file ERR in plenumation, in file %s\n", refFileArch -> file_name);
        return OPENFILEERR;
    }
    refFileArch -> file_size = FileSize(file);
    refFileArch -> buffer = (char*)calloc((size_t)((refFileArch -> file_size) + 1), sizeof(char));

    if (refFileArch -> buffer == NULL) {
        fprintf(stderr, "Calloc error for text_buffer\n");
        return NULLTRANS;
    }
    if (refFileArch -> buffer != NULL) {
        refFileArch -> buffer[(refFileArch -> file_size)] = '\0';
    }

    if ((long long)fread(refFileArch -> buffer, sizeof(char), (size_t)refFileArch -> file_size, file) != refFileArch -> file_size) {
        printf("Read file error\n");                //FIXME - пересмотри типы при работе с big data
        return FREADERR;
    }
    refFileArch -> str_count = (size_t)countSymb(refFileArch -> buffer, '\n', (size_t)refFileArch -> file_size) + 1;
    fclose(file);                                   //FIXME - тоже самое        FIXME
    return NOTRANSERR;
 }



transErr_t Distributor(fileInfo* refFileArch) {
    if (refFileArch -> str_count == 0) return SIZEERR;
    refFileArch -> pointerBuffer = (char**)calloc(refFileArch -> str_count , sizeof(char*));
    if (!(refFileArch -> pointerBuffer)) {
        fprintf(stderr, "Calloc error for ptr_buffer\n");
        free(refFileArch -> pointerBuffer);
        return CALLOCERR;
    }

    size_t stringsAmount = 0;
    refFileArch -> pointerBuffer[stringsAmount++] = refFileArch -> buffer;
    for (size_t i = 0; i < (size_t)refFileArch -> file_size && refFileArch -> buffer[i] != '\0'; i++) {
        if(refFileArch -> buffer[i] == '\n') {
            if (refFileArch -> buffer[i+1] != '\0') {
                refFileArch -> pointerBuffer[stringsAmount++] = &(refFileArch -> buffer[i+1]);
            } else
                break; //что делать с ошибкой?
        }
    }
    return NOTRANSERR;
}



transErr_t ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf) {
    if (!refFileInf || !byteCodeFileInf) return NULLTRANS;
    char arrForWord[99];
    int argument = SWAGVIPERR;

    FILE* fileByteCode = fopen(byteCodeFileInf -> file_name, "w");
    if (!fileByteCode) {
        fprintf(stderr, "File open error - byteCode\n");
        return OPENFILEERR;
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
            } else if (strncmp(arrForWord, "POP", 3) == 0) {
                fprintf(fileByteCode, "2\n");
            } else if (strncmp(arrForWord, "SUM", 3) == 0) {
                fprintf(fileByteCode, "3\n");
            } else if (strncmp(arrForWord, "SUB", 3) == 0) {
                fprintf(fileByteCode, "4\n");
            } else if (strncmp(arrForWord, "MUL", 3) == 0) {
                fprintf(fileByteCode, "5\n");
            } else if (strncmp(arrForWord, "DIV", 3) == 0) {
                fprintf(fileByteCode, "6\n");
            } else if (strncmp(arrForWord, "DUMP", 4) == 0) {
                fprintf(fileByteCode, "7\n");
            } else if (strncmp(arrForWord, "HLT", 3) == 0) {
                fprintf(fileByteCode, "8\n");
            } else {
                fprintf(fileByteCode, "Error in ASM code in string No %zu: \"%s\"\n", k + 1, arrForWord);
                fprintf(stderr, "Error in ASM code in string No %zu: \"%s\"\n", k + 1, arrForWord);
                break;
            }
        }
        fclose(fileByteCode);
    return NOTRANSERR;
}
