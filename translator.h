#ifndef translator_h
#define translator_h

#include <stdio.h>
#include <sys/stat.h>

#include "countSymb.H"
#include "swags.h"

typedef struct{
    const char* file_name;
    size_t str_count;
    long long file_size;
    char* buffer;
    char** pointerBuffer;
 }fileInfo;

enum transErr_t {
    NOTRANSERR = 10,
    OPENFILEERR = 11,
    NULLTRANS = 12,
    FREADERR = 13,
    CALLOCERR = 14,
    SIZEERR = 15
};


long long FileSize(FILE* refFile);
transErr_t Plenumation(fileInfo* refFileArch);
transErr_t Distributor(fileInfo* refFileArch);
transErr_t ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf);

#endif //translator_h
