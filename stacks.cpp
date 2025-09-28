#include <stdio.h>

typedef struct {
    size_t size;
    size_t capacity;
    int* data;
}stack_t;

enum StackErr_t {
    DATANULLPTR = 1,
    INCORRECTSIZE = 2,


}

StackErr_t StackInit(stack_t* refStack, size_t cpcty) {
    refStack -> capasity = cpcty;
    refStack -> data = (int)calloc(cpcty, sizeof(int));
    refStack -> size = 0;
}

StackErr_t StackPush(stack_t* refStack, int val, StackErr_t* err) {
    //assert'ы!!
    *err = StackVerify(refStack);
    refStack -> data[refStack -> size++] = val;
}

int StackPop(stack_t* refStack, size_t ) {
    //assert'ы!!
    return refStack -> data[refStack -> size--];
}


StackErr_t StackDestroy(stack_t* refStack) {
    free(refStack -> data);
    refStack -> data = NULL;
}

StackErr_t StackVerify(stack_t* refStack) {
    if (refStack -> data == NULL)
        return DATANULLPTR;
    if (refStack -> size => refStack -> capasity - 1 || refStack -> size <= 0 || refStack -> capacity <= 0)
        return INCORRECTSIZE;
    if ()
}


int main(void) {
    stack_t stk1 = {};
    StackInit(&stk1, 100);
    StackPush(&stk1, 83);
    StackPush(&stk1, 72);

    
}
