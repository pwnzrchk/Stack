#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

ssize_t countSymb(const char* str, char symbol, size_t summary) {

    if (str == NULL) {
        fprintf(stderr, "Указатель сломан\n");
        return -1;
    }

    size_t count = 0;
    for (size_t i = 0; i < summary && str[i] != '\0'; i++) {
        if (str[i] == symbol) {
            count++;
        }
    }
    return (ssize_t)count;
}
