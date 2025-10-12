#include "translator.h"

const int ComandLenth = 11;
const int RegLenth = 9;

transErr_t ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf) {

    if (!refFileInf || !byteCodeFileInf) return NULL_PTR_TRANSL;

    FILE* fileByteCode = fopen(byteCodeFileInf -> file_name, "w");
    if (!fileByteCode) {
        fprintf(stderr, "File open error - byteCode\n");
        return OPEN_FILE_ERR;
    }
    byteCodeFileInf -> str_count = refFileInf -> str_count;

    size_t borderOfAsm = refFileInf -> str_count - 1;
    size_t argument = 0;
    swagElem_t* bufferArr = (swagElem_t*)calloc(borderOfAsm*2, sizeof(swagElem_t));
    
    for (size_t k = 0; k < borderOfAsm; k++) {  //FIXME отдельная переменная
        char* workline = NULL;
        if (!refFileInf -> pointerBuffer) break;
        workline = refFileInf -> pointerBuffer[k];
        if (!workline) break;

        int comand = funcFinder(workline);
        int reg = regFinder(workline);
        //printf("%s %d\n", workline, comand);

        switch (comand) {
// FIXME - print text to buffer and print it to file just one time at the end
            case PUSH:
                //printf("Str %s\n", workline + 5);
                sscanf(workline + 5, "%zu", &argument);
                fprintf(fileByteCode, "%d %zu\n", PUSH, argument);
                break;

            case POP:
                fprintf(fileByteCode, "%d\n", POP);
                break;

            case SUM:
                fprintf(fileByteCode, "%d\n", SUM);
                break;

            case SUB:
                fprintf(fileByteCode, "%d\n", SUB);
                break;

            case MUL:
                fprintf(fileByteCode, "%d\n", MUL);
                break;

            case DIV:
                fprintf(fileByteCode, "%d\n", DIV);
                break;

            case DUMP:
                fprintf(fileByteCode, "%d\n", DUMP);
                break;

            case HLT:
                fprintf(fileByteCode, "%d\n", HLT);
                break;

            case PUSHR:
                if (reg == ERROR_REG) {
                    fprintf(stderr, "Error reading reg\n");
                    break;
                }
                fprintf(fileByteCode, "%d %d\n", PUSHR, reg);
                break;

            case POPR:
                if (reg == ERROR_REG) {
                    fprintf(stderr, "Error reading reg\n");
                    break;
                }
                fprintf(fileByteCode, "%d %d\n", POPR, reg);
                break;

            case JMP:
                sscanf(workline + 4, "%zu", &argument);
                fprintf(fileByteCode, "%d %zu\n", JMP, argument);


            case ERROR_COM:
            default:
                fprintf(stderr, "WRONG COMAND STR %zu\n", k+1);
                break;
        }
    }
        fclose(fileByteCode);
    return NO_TRANS_ERR;
}


int regFinder (char* refLine) {
    char ArrForReg[RegLenth];
    const char* Registers[] = {"RAX", "RBX", "RCX", "RDX"};

    if (funcFinder(refLine) == PUSHR) {
        sscanf(refLine + 6, "%8s", ArrForReg);
        for (int k = 0; k < (int)(sizeof(Registers)/sizeof(Registers[0])); k++) {
            if (strncmp(ArrForReg, Registers[k], strlen(Registers[k])) == 0) {
                return k + 1;
            }
        }
    }

    if (funcFinder(refLine) == POPR) {
        sscanf(refLine + 5, "%8s", ArrForReg);

        for (int k = 0; k < (int)(sizeof(Registers)/sizeof(Registers[0])); k++) {
            if (strncmp(ArrForReg, Registers[k], strlen(Registers[k])) == 0) {
                return k + 1;
            }
        }
    }
    return ERROR_REG;
}


int funcFinder (char* refLine) {

    char ArrForCom[ComandLenth];
    sscanf(refLine, "%10s", ArrForCom);
    const char* Commands[] = {"PUSH", "POP", "SUM", "SUB",
                        "MUL", "DIV", "DUMP", "HLT", "PUSHR",
                        "POPR", "JMP"};
    for (int i = 0; i < int(sizeof(Commands)/sizeof(Commands[0])); i++) {
        if (strcmp(ArrForCom, Commands[i]) == 0) {
            return i + 1;
        }
    }
    return ERROR_COM;
}

