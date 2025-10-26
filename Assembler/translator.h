#ifndef translator_h
#define translator_h


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../Common/fileFunc.h"
#include "../Common/commands.h"
#include "../Common/swags.h"

//=================================================================================================================================================

#define ComandLenth  11
#define RegLenth 9
#define LABEL_TABEL_SIZE 10
#define SizeOfAddrUsesStack 15
#define LABLE_SIZE 128

enum transErr_t {
    NO_TRANS_ERR         = 10,
    OPEN_FILE_ERR        = 11,
    NULL_PTR_TRANSL      = 12,
    kLabelErr            = 13,
    kErrorCom            = 14,
    kPrinterError        = 15,
    kSizeCalculatorError = 16,
    kBufferFillerError   = 17,
    kPostProcessorError  = 18,
    kFilePrinterError    = 19,
    kBinaryPrinterError  = 20,
    kSizeError           = 21
};

typedef struct {
    char*   name;
    ssize_t addres;
    swag_t  addreses_uses;
}Label_info;

typedef struct {
    int*        Buffer_Arr;
    fileInfo    AsmFile;
    fileInfo    ByteCodeFile;
    fileInfo    binary_file;
    Label_info  Label_Table[LABEL_TABEL_SIZE];
    size_t      label_count;
}Translator;

static const int kInvalidAddress = -1;
static const int kInvalidReturnValue = -1;
static const char kLabelMarker = ':';
static const char kCommentMarker = ';';


//=================================================================================================================================================
//Parsing functions for bytecoder
transErr_t ByteCoder      (Translator* refTranslator);
int funcFinder            (char* refLine);
int regFinder             (char* refLine);
int argFinder             (char* refLine);
char* labelFinder         (char* refLine);
void ErrorHandler         (transErr_t error_code);
transErr_t FilePrinter    (Translator* translator);
transErr_t SizeCalculator (Translator* translator, size_t* calculated_size);
bool NeedArgument         (int cmd);
transErr_t BufferFiller   (Translator* translator);
transErr_t BinaryPrinter  (Translator* translator);

//Functions for work with labels
transErr_t TranslatorConstructor (Translator* refTranslator, fileInfo* refFileInf, fileInfo* byteCodeFileInf);
ssize_t LabelParserCom           (char* refLabel, Translator* refTranslator, size_t Counter_Index);
int LabelParser                  (char* refLabel, Translator* refTranslator, size_t CounterIndex);
bool LoopCmp                     (char const* refLabel, Translator* refTrans, int* index);
transErr_t PostProcessor         (Translator* refTranslator);


#endif //translator_h
