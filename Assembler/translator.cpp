#include "translator.h"


#define JMP_TMPL    if (offset_for_label != NULL) {                                                                             \
                    ssize_t flag_of_existing = 0;                                                                                   \
                        if ((flag_of_existing = LabelParserCom(LabelName, refTranslator, counterIndex)) != kInvalidAddress) {   \
                            refTranslator -> Buffer_Arr[counterIndex++] = (int)flag_of_existing;                                     \
                        } else {                                                                                                \
                            refTranslator -> Buffer_Arr[counterIndex++] = kTrashValue;                                          \
                        }                                                                                                       \
                        break;                                                                                                  \
                    }                                                                                                           \
                    refTranslator -> Buffer_Arr[counterIndex++] = argmnt;                                                       \
                    break;

//=================================================================================================================================================

#define REG_TMPL    if (reg == ERROR_REG) {                             \
                        fprintf(stderr, "Error reading reg\n");         \
                        break;                                          \
                    }                                                   \
                    refTranslator -> Buffer_Arr[counterIndex++] = reg;  \
                    break;

//=================================================================================================================================================

#define CASE_TEMPLATE_ERROR_HANDLER(error_code)                     \
        fprintf(stderr, "%s error in assembling\n", error_code);    \
        break;

//=================================================================================================================================================

transErr_t  ByteCoder(Translator* refTranslator) {

    if (!(refTranslator -> AsmFile.file_name) || !(refTranslator -> ByteCodeFile.file_name)) return NULL_PTR_TRANSL;

    FILE* fileByteCode = fopen(refTranslator -> ByteCodeFile.file_name, "w");
    if (!fileByteCode) {
        fprintf(stderr, "File open error - byteCode\n");
        return OPEN_FILE_ERR;
    }
    refTranslator->ByteCodeFile.str_count = refTranslator->AsmFile.str_count;

    size_t borderOfAsm = refTranslator->AsmFile.str_count - 4;  //FIXME - откуда?
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

        if(workline[0] == '\0') continue;

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
            case RET:
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
            case CALL:
                refTranslator -> Buffer_Arr[counterIndex++] = comand;
                JMP_TMPL

            case LABEL:
                if (LabelParser(LabelName, refTranslator, counterIndex) == -1) return kLabelErr;
            break;

            case ERROR_COM:
            default:
                fprintf(stderr, "WRONG COMAND STR %zu\n", i+1);
                return kErrorCom;
        }
    }

    PostProcessor(refTranslator);

    FilePrinter(refTranslator, fileByteCode);

    fclose(fileByteCode);
    return NO_TRANS_ERR;
}

//=================================================================================================================================================
//Будет считать количество команд двухаргументных и одноаргументных и потом realloc под точный размер. В два пробега подсчет: кол-ва элементов и потом запись в реаллокнутое
transErr_t FilePrinter(Translator* translator, FILE* printable_file) {

    size_t memory_size = 0;
    if (SizeCalculator(translator, &memory_size) != NO_TRANS_ERR) return kSizeCalculatorError;

    int* template_buffer = (int*)calloc(memory_size, sizeof(int));
    memcpy(template_buffer, translator->Buffer_Arr, memory_size * sizeof(int));
    free(translator->Buffer_Arr);
    translator->Buffer_Arr = template_buffer;

    printf("Memory size = %zu\n", memory_size);

    for (size_t i = 0; i < memory_size; i++) {
        fprintf(printable_file, "%d", translator->Buffer_Arr[i]);
        // printf("[%zu] %d\n", i, translator->Buffer_Arr[i]);

        int cmd = translator->Buffer_Arr[i];
        if (cmd == POP || cmd == SUM || cmd == SUB || cmd == MUL || cmd == DIV || cmd == DUMP || cmd == HLT || cmd == RET)
            fprintf(printable_file, "\n");
        else
            fprintf(printable_file, " %d\n", translator->Buffer_Arr[++i]);
    }

    return NO_TRANS_ERR;
}

//=================================================================================================================================================

transErr_t SizeCalculator(Translator* translator, size_t* calculated_size) {

    size_t original_size = kBufferMultiplier * (translator->AsmFile.str_count - 1);
    size_t memory_size = 0;

    for (size_t i = 0; i < original_size && translator->Buffer_Arr[i] != 0; i++) {
        int cmd = translator->Buffer_Arr[i];
        if (cmd == ERROR_COM) return kSizeCalculatorError;
        // printf("%d Comand - %d\n", i, cmd);
        switch (cmd) {
            case POP: case SUM: case SUB: case MUL: case DIV:
            case DUMP: case HLT: case RET:
                memory_size += 1;
                break;
            default:
                memory_size += 2;
                i++;
                break;
        }
    }

    *calculated_size = memory_size;
    return NO_TRANS_ERR;
}

//=================================================================================================================================================

int regFinder (char* refLine) {
    assert(refLine);

    char ArrForReg[RegLenth];
    const char* Registers[] = {"RAX", "RBX", "RCX", "RDX"};
    char* reg_offset = strchr(refLine, ' ');
    sscanf(reg_offset, "%8s", ArrForReg);
    for (int i = 0; i < (int)(sizeof(Registers)/sizeof(Registers[0])); i++) {
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
                              "JNE", "CALL", "RET"};
    for (int i = 0; i < (int)(sizeof(Commands)/sizeof(Commands[0])); i++) {
        if ((strcmp(ArrForCom, Commands[i])) == 0) {
            return i + 1;
        }
    }
    return ERROR_COM;
}

//=================================================================================================================================================

int argFinder (char* refLine) {
    if (refLine == NULL) return kTrashValue;
    int arg = kTrashValue;
    if (sscanf(refLine, "%*s %d", &arg) == 1) {
        return arg;
    }
    return kTrashValue;
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
ssize_t LabelParserCom(char* refLabel, Translator* refTranslator, size_t Counter_Index) {
    int index = kInvalidAddress;
    bool FlagOfExisting = LoopCmp(refLabel, refTranslator, &index);

    if (FlagOfExisting) {
        if (refTranslator -> Label_Table[index].addres != kInvalidAddress) {
            return refTranslator ->Label_Table[index].addres;
        } else  {
            SwagPush(&(refTranslator -> Label_Table[index].addreses_uses), Counter_Index + 1);
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
        if (refTranslator ->Label_Table[index].addres != kInvalidAddress) {
            fprintf(stderr, "Redefinition of label\n");
            return -1;
        } else if (refTranslator ->Label_Table[index].addres == kInvalidAddress){
            refTranslator ->Label_Table[index].addres = (ssize_t)CounterIndex;
            return 0;
        }
    }

    refTranslator->Label_Table[refTranslator -> label_count].name = refLabel;
    refTranslator->Label_Table[refTranslator -> label_count].addres = (ssize_t)CounterIndex;
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
    for (size_t i = 0; i < refTranslator ->label_count; i++) {
        // printf("Labels count: %d\n", refTranslator ->label_count);
        swagElem_t addres = 0;
        size_t Addresse_used_Size = refTranslator->Label_Table[i].addreses_uses.size;
        // printf("Addresse_used_Size is %zu\n", Addresse_used_Size);
        for (size_t k = 0; k < Addresse_used_Size; k++) {
            SwagPop(&refTranslator->Label_Table[i].addreses_uses, &addres);
            refTranslator->Buffer_Arr[addres] = (int)refTranslator->Label_Table[i].addres;
        }
    }
    return NO_TRANS_ERR;
}

//=================================================================================================================================================

void ErrorHandler(transErr_t error_code) {
    switch(error_code) {
        case OPEN_FILE_ERR:
            CASE_TEMPLATE_ERROR_HANDLER("Open file")

        case NULL_PTR_TRANSL:
            CASE_TEMPLATE_ERROR_HANDLER("Null pointer")

        case kLabelErr:
            CASE_TEMPLATE_ERROR_HANDLER("Label parsing")

        case kErrorCom:
            CASE_TEMPLATE_ERROR_HANDLER("Comand input")

        case kPrinterError:
            CASE_TEMPLATE_ERROR_HANDLER("Print to file")

        case kSizeCalculatorError:
            CASE_TEMPLATE_ERROR_HANDLER("Calculation size of bytecode")

        case NO_TRANS_ERR:
        default:
            fprintf(stderr, "ErrorHandler error\nUnknown code of error\n");
    }

}

//=================================================================================================================================================
