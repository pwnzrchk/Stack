#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swags.h"


swagErr_t SwagVerify(swag_t* refSwag) {
    if (refSwag == NULL || refSwag -> data == NULL)
        return DATA_NULL_PTR;
    if (refSwag -> capacity == 0)
        return INCORRECT_SIZE;
    if (refSwag -> size > refSwag -> capacity)
        return SWAG_OVERFLOW;
    if (refSwag -> data[0] != FIRSTVIPERR || refSwag -> data[refSwag -> capacity + 1] != SECONDVIPERR)
        return DIED_BIRDS;
    return NO_ERRS;
}

//=================================================================================================================================================

swagErr_t SwagInit(swag_t* refSwag, size_t cpcty) {

    if(refSwag == NULL)
        return DATA_NULL_PTR;

    if(cpcty <= 0)
        return INCORRECT_SIZE;

    refSwag -> capacity = cpcty;
    refSwag -> data = (swagElem_t*)calloc(cpcty + 2, sizeof(swagElem_t));
    refSwag -> size = 0;
    for (size_t i = 0; (size_t)i < cpcty; i++) {
        refSwag -> data[(refSwag -> size) + (size_t)i + 1] = SWAGVIPERR;
    }
    refSwag -> data[0] = FIRSTVIPERR;
    refSwag -> data[cpcty + 1] = SECONDVIPERR;
    return SwagVerify(refSwag);
}

//=================================================================================================================================================

swagErr_t SwagPush(swag_t* refSwag, swagElem_t val) {

    if (SwagVerify(refSwag) != NO_ERRS)
        return SwagVerify(refSwag);


    if (refSwag -> size < refSwag -> capacity) {
        refSwag -> data[(refSwag -> size++) + 1] = val;
        return SwagVerify(refSwag);
    }

    static int over = 0;
    refSwag -> capacity *= 2;
    swagElem_t* temp = (swagElem_t*)realloc(refSwag -> data, (refSwag -> capacity + 2) * sizeof(swagElem_t));
    if (temp == NULL) {
        fprintf(stderr, "Realloc error\n");
        return REALLOC_ERR;
    }

    refSwag -> data = temp;
    for (size_t i = 0; i < refSwag -> capacity - (refSwag -> size); i++) {
        refSwag -> data[(refSwag -> size) + (size_t)i + 1] = SWAGVIPERR;
    }
    refSwag -> data[0] = FIRSTVIPERR;
    refSwag -> data[(refSwag -> capacity) + 1] = SECONDVIPERR;
    fprintf(stderr, "Swag overflow No%d\n", ++over);
    refSwag -> data[(refSwag -> size++) + 1] = val;
    return SwagVerify(refSwag);
}

//=================================================================================================================================================

swagErr_t SwagPop(swag_t* refSwag, swagElem_t* popped) {
    if (SwagVerify(refSwag) != NO_ERRS) {
        return SwagVerify(refSwag);
    }
    if (refSwag -> size == 0) {
        return INCORRECT_SIZE;
    }
    *popped = refSwag -> data[(refSwag -> size)--];
    refSwag -> data[(refSwag -> size) + 1] = SWAGVIPERR;
    return SwagVerify(refSwag);
}

//=================================================================================================================================================

swagErr_t SwagDestroy(swag_t* refSwag) {
    if (SwagVerify(refSwag) != NO_ERRS) {
        return SwagVerify(refSwag);
    }
    free(refSwag -> data);
    refSwag -> data = NULL;
    refSwag -> capacity = 0;
    refSwag -> size = 0;
    return NO_ERRS;
}

//=================================================================================================================================================

void SwagDump(swag_t* refSwag, swagErr_t mistake) {
    printf("\n\n----------------S.W.A.G.G.A----------------\n\n");
    printf("Swag pointer: %10p\n", refSwag);
    if (refSwag == NULL)
        return;
    printf("Swag size:    %10lu\n", refSwag -> size);
    printf("Swag capacity:%10lu\n", refSwag -> capacity);
    printf("\nAddres FIRST_VIPERR: %15p \t Addres SECOND_VIPERR: %15p\n", refSwag -> data, refSwag -> data + (refSwag -> capacity + 1));
    if(refSwag -> data == NULL)
        return;
    printf("FIRST_VIPERR: %3zu \t SECOND_VIPERR: %3zu\n\n", refSwag -> data[0], refSwag -> data[refSwag -> capacity + 1]);
    printf("Cells of the swag:\n");

    if (mistake != DATA_NULL_PTR) {
        for (size_t i = 0; i < refSwag->capacity; i++) {
            printf("%10zu ", refSwag->data[1 + i]);
            if ((i + 1) % 3 == 0) {
                printf("\n");
            }
        }
    }

    if (mistake != NO_ERRS) {
        printf("\n\nErrorNo: %d", mistake);
    }

    printf("\n\n----------------S.W.A.G.G.A----------------\n\n");
}

//=================================================================================================================================================

swagErr_t SwagSumn(swag_t* refSwag) {
    assert(refSwag != NULL);
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NO_ERRS;
    if ((error = SwagVerify(refSwag)) != NO_ERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if (((errorFirst = SwagPop(refSwag, &arg1)) != NO_ERRS) || ((errorSecond = SwagPop(refSwag, &arg2)) != NO_ERRS)) {
        error = (errorFirst != NO_ERRS ? errorFirst : errorSecond);
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 + arg2)) != NO_ERRS) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}

//=================================================================================================================================================

swagErr_t SwagSub(swag_t* refSwag) {
    assert(refSwag != NULL);
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NO_ERRS;
    if ((error = SwagVerify(refSwag)) != NO_ERRS) {
        SwagDump(refSwag, error);
        fprintf(stderr, "Error 1 in SUB");
        return error;
    }

    if ((errorFirst = SwagPop(refSwag, &arg1)) != NO_ERRS || ((errorSecond = SwagPop(refSwag, &arg2)) != NO_ERRS)) {
        error = (errorFirst != NO_ERRS ? errorFirst : errorSecond);
        SwagDump(refSwag, error);
        fprintf(stderr, "Error 2 in SUB");
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 - arg2)) != NO_ERRS) {
        SwagDump(refSwag, error);
        fprintf(stderr, "Error 3 in SUB");
        return error;
    }
    return SwagVerify(refSwag);
}

//=================================================================================================================================================

swagErr_t SwagMul(swag_t* refSwag) {
    assert(refSwag != NULL);
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NO_ERRS;
    if ((error = SwagVerify(refSwag)) != NO_ERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if ((errorFirst = SwagPop(refSwag, &arg1)) != NO_ERRS || ((errorSecond = SwagPop(refSwag, &arg2)) != NO_ERRS)) {
        error = (errorFirst != NO_ERRS ? errorFirst : errorSecond);
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 * arg2)) != NO_ERRS) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}

//=================================================================================================================================================

swagErr_t SwagDiv(swag_t* refSwag) {
    assert(refSwag != NULL);
    swagElem_t arg1, arg2 = SWAGVIPERR;
    swagErr_t error, errorFirst, errorSecond = NO_ERRS;
    if ((error = SwagVerify(refSwag)) != NO_ERRS) {
        SwagDump(refSwag, error);
        return error;
    }

    if ((errorFirst = SwagPop(refSwag, &arg1)) != NO_ERRS || ((errorSecond = SwagPop(refSwag, &arg2)) != NO_ERRS) || arg2 == 0) {
        error = (errorFirst != NO_ERRS) ? errorFirst : (errorSecond != NO_ERRS) ? errorSecond : NULL_DIVISION;
        SwagDump(refSwag, error);
        return error;
    }
    if ((error = SwagPush(refSwag, arg1 / arg2)) != NO_ERRS ) {
        SwagDump(refSwag, error);
        return error;
    }
    return SwagVerify(refSwag);
}

//=================================================================================================================================================
