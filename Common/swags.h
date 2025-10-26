#ifndef swags_h
#define swags_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//Бля беспорядок ебаный как хорошо когда пишешь документацию и читаемый код а не бардак с неймингом банально
const int SWAGVIPERR = 8008135;
const int FIRSTVIPERR = 13;
const int SECONDVIPERR = 9;

enum swagErr_t {
    NO_ERRS = 0,
    DATA_NULL_PTR = 1,
    INCORRECT_SIZE = 2,
    FULL_SWAG = 3,
    SWAG_OVERFLOW = 4,
    DIED_BIRDS = 5,
    REALLOC_ERR = 6,
    NULL_DIVISION = 7
};

typedef size_t swagElem_t;

typedef struct swag_t {
    swagElem_t size;
    swagElem_t capacity;
    swagElem_t* data;
}swag_t;

//Main functions for stack
swagErr_t SwagInit(swag_t* refSwag, size_t cpcty);
swagErr_t SwagPush(swag_t* refSwag, swagElem_t val);
swagErr_t SwagPop(swag_t* refSwag, swagElem_t* popped);
swagErr_t SwagVerify(swag_t* refSwag);
swagErr_t SwagDestroy(swag_t* refSwag);
void SwagDump(swag_t* refSwag, swagErr_t mistake);
//Side functoins for stack
swagErr_t SwagSumn(swag_t* refSwag);
swagErr_t SwagSub(swag_t* refSwag);
swagErr_t SwagMul(swag_t* refSwag);
swagErr_t SwagDiv(swag_t* refSwag);


#endif // swags_h
