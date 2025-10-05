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

long long FileSize(FILE* refFile);
void Plenumation(fileInfo* refFileArch);
void Distributor(fileInfo* refFileArch);
void ByteCoder(fileInfo* refFileInf, fileInfo* byteCodeFileInf);

#endif //translator_h
