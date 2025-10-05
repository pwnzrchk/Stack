#ifndef proccesor_h
#define proccesor_h

#include <stdio.h>
#include <stdbool.h>

#include "swags.h"
#include "translator.h"

enum ProcErr_t {
    WITHOUTERRS = 0,
    NULLPTRERR = 1
};


ProcErr_t Proccesing(fileInfo* BCFile, swag_t* refSwag);

#endif //proccesor_h
