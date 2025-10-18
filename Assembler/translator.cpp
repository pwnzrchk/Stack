#include "translator.h"


#define JMP_TMPL    if (offset != NULL) {                                                                         \
                        if ((flag = LabelParserCom(LabelName, refTranslator, counterIndex)) != kInvalidAddress) { \
                            refTranslator -> Buffer_Arr[counterIndex++] = flag;                                   \
                        } else {                                                                                  \
                            refTranslator -> Buffer_Arr[counterIndex++] = TRASH;                                  \
                        }                                                                                         \
                        break;                                                                                    \
                    }                                                                                             \
                    refTranslator -> Buffer_Arr[counterIndex++] = argmnt;                                         \
                    break;

//=================================================================================================================================================

#define REG_TMPL    if (reg == ERROR_REG) {                             \
                        fprintf(stderr, "Error reading reg\n");         \
                        break;                                          \
                    }                                                   \
                    refTranslator -> Buffer_Arr[counterIndex++] = reg;  \
                    break;

//=================================================================================================================================================

transErr_t ByteCoder(Translator* refTranslator) {

    if (!(refTranslator -> AsmFile.file_name) || !(refTranslator -> ByteCodeFile.file_name)) return NULL_PTR_TRANSL;

    FILE* fileByteCode = fopen(refTranslator -> ByteCodeFile.file_name, "w");
    if (!fileByteCode) {
        fprintf(stderr, "File open error - byteCode\n");
        return OPEN_FILE_ERR;
    }
    refTranslator->ByteCodeFile.str_count = refTranslator->AsmFile.str_count;

    size_t borderOfAsm = refTranslator->AsmFile.str_count - 1;
    refTranslator -> Buffer_Arr = (int*)calloc(borderOfAsm*kBufferMultiplier, sizeof(int));
    size_t counterIndex = 0;

    for (size_t i = 0; i < borderOfAsm; i++) {
        if (!refTranslator->AsmFile.pointerBuffer) break;

        char* workline = refTranslator->AsmFile.pointerBuffer[i];
        if (!workline) break;

        int comand = funcFinder(workline);
        int reg = ERROR_REG;
        if (comand == POPR || comand == PUSHR) reg = regFinder(workline);
        int argmnt = argFinder(workline);

        char* LabelName = NULL;
        char* offset_for_label = strchr(workline, ':');
        if (offset_for_label != NULL) {
            LabelName = labelFinder(offset_for_label + 1);
        }
        int flag = 0;


        switch (comand) {
            case PUSH:
                refTranslator -> Buffer_Arr[counterIndex++] = comand;
                refTranslator -> Buffer_Arr[counterIndex++] = argmnt;
                break;

            case POP:
            case SUM:
            case SUB:
            case MUL:
            case DIV:
            case DUMP:
            case HLT:
                refTranslator -> Buffer_Arr[counterIndex++] = comand;
                break;

            case PUSHR:
            case POPR:
                refTranslator -> Buffer_Arr[counterIndex++] = comand;
                REG_TMPL

            case JMP:
            case JB:
            case JBE:
            case JA:
            case JAE:
            case JE:
            case JNE:
                refTranslator -> Buffer_Arr[counterIndex++] = comand;
                JMP_TMPL

            case LABEL:
                if (LabelParser(LabelName, refTranslator, counterIndex) == -1) fprintf(stderr, "Label ERR\n");
                return kLabelErr;
            break;

            case ERROR_COM:
            default:
                fprintf(stderr, "WRONG COMAND STR %zu\n", i+1);
                return kErrorCom;
        }
    }

    PostProcessor(refTranslator);

    //FIXME -  function Output (call in main)

    for (size_t j = 0; j < borderOfAsm*kBufferMultiplier - 1; j++) {
        fprintf(fileByteCode, "%d", refTranslator -> Buffer_Arr[j]);
        if (refTranslator -> Buffer_Arr[j] == ERROR_COM) {
            fprintf(stderr, "Error comand on %zu line\n", j+1);
            break;
        }
        if (refTranslator -> Buffer_Arr[j] > PUSH && refTranslator -> Buffer_Arr[j] < PUSHR) {
            fprintf(fileByteCode, "\n");
        } else {
            fprintf(fileByteCode, " %d\n", refTranslator -> Buffer_Arr[++j]);
        }
    }

    fclose(fileByteCode);
    return NO_TRANS_ERR;
}

//=================================================================================================================================================

int regFinder (char* refLine) {
    assert(refLine);

    char ArrForReg[RegLenth];
    const char* Registers[] = {"RAX", "RBX", "RCX", "RDX"};
    char* reg_offset = strchr(refLine, ' ');
    sscanf(reg_offset, "%8s", ArrForReg);
    for (int i = 0; i < (int)(sizeof(Registers)/sizeof(Registers[0])); k++) {
        if (strncmp(ArrForReg, Registers[i], strlen(Registers[i])) == 0) {
            return i + 1;
        }
    }
    return ERROR_REG;
}

//=================================================================================================================================================

int funcFinder (char* refLine) {
    if (refLine[0] == ':') return LABEL;
    char ArrForCom[ComandLenth];
    sscanf(refLine, "%10s", ArrForCom);
    const char* Commands[] = {"PUSH", "POP", "SUM", "SUB",
                              "MUL", "DIV", "DUMP", "HLT",
                              "PUSHR", "POPR", "JMP", "JB",
                              "JBE", "JA", "JAE", "JE",
                              "JNE", ":"};
    for (int i = 0; i < (int)(sizeof(Commands)/sizeof(Commands[0])); i++) {
        if ((strcmp(ArrForCom, Commands[i])) == 0) {
            return i + 1;
        }
    }
    return ERROR_COM;
}

//=================================================================================================================================================

int argFinder (char* refLine) {
    if (refLine == NULL) return TRASH;
    int arg = TRASH;
    if (sscanf(refLine, "%*s %d", &arg) == 1) {
        return arg;
    }
    return TRASH;
}

//=================================================================================================================================================

char* labelFinder(char* refLine) {
    char temp[LABLE_SIZE];
    sscanf(refLine, "%127s", temp);
    char* Buffer = (char*)calloc(strlen(temp) + 1, sizeof(char));
    strcpy(Buffer, temp);
    return Buffer;
}

//===============================================================================================================================================
//REVIEW - With flag (boolean value) is clearer what is doing??
int LabelParserCom(char* refLabel, Translator* refTranslator, size_t Counter_Index) {
    int index = kInvalidAddress;
    bool FlagOfExisting = LoopCmp(refLabel, refTranslator, &index);

    if (FlagOfExisting) {
        if (refTranslator -> Label_Table[index].addres != kInvalidAddress) {
            return refTranslator ->Label_Table[index].addres;
        } else if (refTranslator ->Label_Table[index].addres == kInvalidAddress) {
            SwagPush(&(refTranslator -> Label_Table[index].addreses_uses), Counter_Index + 1);
            return -1;
        } else {
            fprintf(stderr, "Incorrect addres of %d element in Lable_table is %d\n", index, refTranslator ->Label_Table[index].addres);
            return kInvalidReturnValue;
        }
    }

    if (!FlagOfExisting) {
        refTranslator->Label_Table[refTranslator -> label_count].name = refLabel;
        refTranslator->Label_Table[refTranslator -> label_count].addres = kInvalidAddress;

        SwagInit(&refTranslator->Label_Table[refTranslator -> label_count].addreses_uses, SizeOfAddrUsesStack);
        SwagPush(&(refTranslator -> Label_Table[refTranslator -> label_count].addreses_uses), Counter_Index + 1);
        refTranslator -> label_count++;
        return kInvalidReturnValue;
    }
    return kInvalidReturnValue;
}

//=================================================================================================================================================

bool LoopCmp(char const* refLabel, Translator* refTrans, int* index) {
    for (int i = 0; i < (int)(sizeof(refTrans -> Label_Table)/sizeof(refTrans->Label_Table[0])); i++) {
        if (!(refTrans -> Label_Table[i].name == NULL) && !strcmp(refLabel, refTrans -> Label_Table[i].name)) {
            *index = i;
            return true;
        }
    }
    return false;
}

//=================================================================================================================================================

int LabelParser(char* refLabel, Translator* refTranslator, size_t CounterIndex) {
    int index = -1;
    bool FlagOfExisting = LoopCmp(refLabel, refTranslator, &index);

    if (FlagOfExisting) {
        if (refTranslator ->Label_Table[index].addres != -1) {
            fprintf(stderr, "Redefinition of label\n");
            return -1;
        } else if (refTranslator ->Label_Table[index].addres == -1){
            refTranslator ->Label_Table[index].addres = CounterIndex;
            return 0;
        }
    }

    refTranslator->Label_Table[refTranslator -> label_count].name = refLabel;
    refTranslator->Label_Table[refTranslator -> label_count].addres = CounterIndex;
    SwagInit(&refTranslator->Label_Table[refTranslator -> label_count].addreses_uses, SizeOfAddrUsesStack);
    refTranslator -> label_count++;
    return 0;
}

//=================================================================================================================================================

transErr_t TranslatorConstructor(Translator* refTranslator, fileInfo* refFileInf, fileInfo* byteCodeFileInf) {
    Plenumation(refFileInf);
    Distributor(refFileInf);
    for (int i = 0; i < LABEL_TABEL_SIZE; i++) {
        refTranslator->Label_Table[i].name = NULL;
        refTranslator->Label_Table[i].addres = -1;
        // Можно ещё SwagInit, если нужно инициализировать стек адресов
    }

    refTranslator -> Buffer_Arr = NULL;
    refTranslator -> AsmFile = *refFileInf;
    refTranslator -> ByteCodeFile = *byteCodeFileInf;
    refTranslator -> label_count = 0;
    return NO_TRANS_ERR;
}

//=================================================================================================================================================

transErr_t PostProcessor (Translator* refTranslator) {
    for (int i = 0; i < refTranslator ->label_count; i++) {
        printf("Labels count: %d\n", refTranslator ->label_count);
        swagElem_t addres = 0;
        size_t Addresse_used_Size = refTranslator->Label_Table[i].addreses_uses.size;
        printf("Addresse_used_Size is %zu\n", Addresse_used_Size);
        for (int k = 0; k < Addresse_used_Size; k++) {
            SwagPop(&refTranslator->Label_Table[i].addreses_uses, &addres);
            refTranslator->Buffer_Arr[addres] = refTranslator->Label_Table[i].addres;
        }
    }
    return NO_TRANS_ERR;
}
