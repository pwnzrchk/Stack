#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SWAGVIPERR 8008135
#define FIRSTVIPERR 13
#define SECONDVIPERR 9
typedef int swagElem_t;

typedef struct swag_t {
    size_t size;
    size_t capacity;
    int* data;
}swag_t;

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

swagErr_t SwagVerify(swag_t* refSwag) {
    if (refSwag == NULL || refSwag -> data == NULL)
        return DATANULLPTR;
    if (refSwag -> size < 0 || refSwag -> capacity <= 0)
        return INCORRECTSIZE;
    if (refSwag -> size > refSwag -> capacity)
        return SWAGOVERFLOW;
    if (refSwag -> data[0] != FIRSTVIPERR || refSwag -> data[refSwag -> capacity + 1] != SECONDVIPERR)
        return DIEDBIRDS;
    return NOERRS;
}


swagErr_t SwagInit(swag_t* refSwag, size_t cpcty) {

    if(refSwag == NULL)
        return DATANULLPTR;

    if(cpcty <= 0)
        return INCORRECTSIZE;

    refSwag -> capacity = cpcty;
    refSwag -> data = (swagElem_t*)calloc(cpcty + 2, sizeof(swagElem_t));
    refSwag -> size = 0;
    for (int i = 0; i < cpcty; i++) {
        refSwag -> data[(refSwag -> size) + i + 1] = SWAGVIPERR;
    }
    refSwag -> data[0] = FIRSTVIPERR;
    refSwag -> data[cpcty + 1] = SECONDVIPERR;
    return SwagVerify(refSwag);
}

swagErr_t SwagPush(swag_t* refSwag, swagElem_t val) {

    if (SwagVerify(refSwag) != NOERRS)
        return SwagVerify(refSwag);

    static int over = 0;
    if (refSwag -> size < refSwag -> capacity) {
        refSwag -> data[(refSwag -> size++) + 1] = val;
        return SwagVerify(refSwag);
    }

    refSwag -> capacity *= 2;
    swagElem_t* temp = (swagElem_t*)realloc(refSwag -> data, 2*(refSwag -> capacity) + 2);
    if (temp == NULL) {
        fprintf(stderr, "Realloc error\n");
        return REALLOCERR;
    }

    refSwag -> data = temp;
    for (int i = 0; i < refSwag -> capacity - (refSwag -> size); i++) {
        refSwag -> data[(refSwag -> size) + i] = SWAGVIPERR;
    }
    refSwag -> data[0] = FIRSTVIPERR;
    refSwag -> data[refSwag -> capacity + 1] = SECONDVIPERR;
    fprintf(stderr, "перегруз свэга бозо №%d\n", ++over);
    refSwag -> data[(refSwag -> size++) + 1] = val;
    return SwagVerify(refSwag);
}


swagErr_t SwagPop(swag_t* refSwag, swagElem_t* popped) {
    if (SwagVerify(refSwag) != NOERRS) {
        return SwagVerify(refSwag);
    }
    if (refSwag -> size == 0) {
        return INCORRECTSIZE;
    }
    *popped = refSwag -> data[(refSwag -> size)--];
    refSwag -> data[(refSwag -> size) + 1] = SWAGVIPERR;
    return SwagVerify(refSwag);
}


swagErr_t SwagDestroy(swag_t* refSwag) {
    if (SwagVerify(refSwag) != NOERRS) {
        return SwagVerify(refSwag);
    }
    free(refSwag -> data);
    refSwag -> data = NULL;
    refSwag -> capacity = SWAGVIPERR;
    refSwag -> size = SWAGVIPERR;
    return NOERRS;
}


void SwagDump(swag_t* refSwag, swagErr_t mistake) {
printf("\n\n----------------S.W.A.G.G.A----------------\n\n");
printf("Swag pointer: %p\n", refSwag);
if (refSwag == NULL)
    return;
printf("Swag size: %lu\n", refSwag -> size);
printf("Swag capacity: %lu\n", refSwag -> capacity);
printf("\nAddres FIRST_VIPERR: %p \t Addres SECOND_VIPERR: %p\n\n", refSwag -> data, refSwag -> data + (refSwag -> capacity + 1));
if(refSwag -> data == NULL)
    return;
printf("FIRST_VIPERR: %d \t SECOND_VIPERR: %d\n\n", refSwag -> data[0], refSwag -> data[refSwag -> capacity + 1]);
printf("Cells of the swag:\n");

if (mistake != DATANULLPTR) {
    for (int i = 0; i < refSwag->capacity - 1; i++) {
        printf("%10d ", refSwag->data[1 + i]);
        if ((i + 1) % 3 == 0) {
            printf("\n");
        }
    }
}

if (mistake != NOERRS) {
    printf("\n\nErrorNo: %d", mistake);
}

printf("\n\n----------------S.W.A.G.G.A----------------\n\n");
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
swagErr_t SwagSumn(swag_t* refSwag) {
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NOERRS;
    if ((error = SwagVerify(refSwag)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if (((errorFirst = SwagPop(refSwag, &arg1)) != NOERRS) || ((errorSecond = SwagPop(refSwag, &arg2)) != NOERRS)) {
        error = (errorFirst != NOERRS ? errorFirst : errorSecond);
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 + arg2)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}


swagErr_t SwagSub(swag_t* refSwag) {
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NOERRS;
    if ((error = SwagVerify(refSwag)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if ((errorFirst = SwagPop(refSwag, &arg1)) != NOERRS || ((errorSecond = SwagPop(refSwag, &arg2)) != NOERRS)) {
        error = (errorFirst != NOERRS ? errorFirst : errorSecond);
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 - arg2)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}


swagErr_t SwagMul(swag_t* refSwag) {
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NOERRS;
    if ((error = SwagVerify(refSwag)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if ((errorFirst = SwagPop(refSwag, &arg1)) != NOERRS || ((errorSecond = SwagPop(refSwag, &arg2)) != NOERRS)) {
        error = (errorFirst != NOERRS ? errorFirst : errorSecond);
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 * arg2)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}


swagErr_t SwagDiv(swag_t* refSwag) {
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NOERRS;
    if ((error = SwagVerify(refSwag)) != NOERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if ((errorFirst = SwagPop(refSwag, &arg1)) != NOERRS || ((errorSecond = SwagPop(refSwag, &arg2)) != NOERRS) || arg2 == 0) {
        error = (errorFirst != NOERRS) ? errorFirst : (errorSecond != NOERRS) ? errorSecond : NULLDIVISION;
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 / arg2)) != NOERRS ) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}
