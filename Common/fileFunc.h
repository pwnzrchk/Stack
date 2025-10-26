#ifndef fileFunc_h
#define fileFunc_h


#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>

#include "countSymb.h"

//=================================================================================================================================================

enum fileFunErr_t {
    NO_PLUM_ERR =     0,
    OPEN_PLUM_ERR =   1,
    NULL_PTR_PLUM =   2,
    FREAD_PLUM_ERR =  3,
    CALLOC_PLUM_ERR = 4,
    INC_PLUM_SIZE =   5
};

typedef struct{
    const char* file_name;
    size_t      str_count;
    long long   file_size;
    char*       buffer;
    char**      pointerBuffer;
 }fileInfo;


fileFunErr_t Plenumation(fileInfo* refFileArch);
fileFunErr_t Distributor(fileInfo* refFileArch);
long FileSize           (FILE* file);


#endif  //fileFunc_h
