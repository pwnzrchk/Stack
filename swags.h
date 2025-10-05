#ifndef swags_h
#define swags_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAGVIPERR 8008135
#define FIRSTVIPERR 13
#define SECONDVIPERR 9

enum swagErr_t {
    NOERRS = 0,
    DATANULLPTR = 1,
    INCORRECTSIZE = 2,
    FULLSWAG = 3,
    SWAGOVERFLOW = 4,
    DIEDBIRDS = 5,
    REALLOCERR = 6,
    NULLDIVISION = 7

};

typedef struct swag_t {
    size_t size;
    size_t capacity;
    int* data;
}swag_t;

typedef int swagElem_t;

swagErr_t SwagInit(swag_t* refSwag, size_t cpcty);
swagErr_t SwagPush(swag_t* refSwag, swagElem_t val);
swagErr_t SwagPop(swag_t* refSwag, swagElem_t* popped);
swagErr_t SwagVerify(swag_t* refSwag);
swagErr_t SwagDestroy(swag_t* refSwag);
void SwagDump(swag_t* refSwag, swagErr_t mistake);

swagErr_t SwagSumn(swag_t* refSwag);
swagErr_t SwagSub(swag_t* refSwag);
swagErr_t SwagMul(swag_t* refSwag);
swagErr_t SwagDiv(swag_t* refSwag);


#endif // swags_h
