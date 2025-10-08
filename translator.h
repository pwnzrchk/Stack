#ifndef translator_h
#define translator_h

#include <stdio.h>
#include <string.h>


#include "fileFunc.h"


enum transErr_t {
    NO_TRANS_ERR = 10,
    OPEN_FILE_ERR = 11,
    NULL_PTR_TRANSL = 12,
};


transErr_t ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf);

#endif //translator_h
