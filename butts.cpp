#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "swags.h"

int main(void) {

    swag_t swag0 = {};
    size_t enteredSize = 0;
    swagErr_t errorInit = NOERRS;
    printf("HOLA!\nEnter capacity of your stack\n");
    if (scanf("%zd", &enteredSize) != 1) {
        printf("Wrong input\n");
        return 0;
    } else {
        char ch = getchar();
        if ((errorInit = SwagInit(&swag0, enteredSize)) == NOERRS) {
            printf("Created new SWAG with capacity of %zu\n", enteredSize);
        } else {
            printf("Initialization error\n");
            SwagDump(&swag0, errorInit);
            return 0;
        }
    }
    SwagInit(&swag0, enteredSize);


    bool FlagOfExit = false;
    bool* FlagOfExitPTR = &FlagOfExit;
    char* buffer = NULL;
    size_t bufferSize = 0;


    while(!FlagOfExit) {
        printf("Commands:\n1 - PUSH 2 - POP 3 - SUM\n4 - SUB 5 - MUL 6- DIV\n7 - DUMP\n8 - HLT\n");
        printf("Enter in format: COMMAND(num) - value(any int if w/0 arguments)\n\n");
        size_t command = 0;
        swagType value, popped1, popped2 = SWAGVIPERR;

        swagErr_t error = NOERRS;
        int str_len = 0;

        if((str_len = (int)getline(&buffer, &bufferSize, stdin)) == -1) {
            printf("Wrong input\n");
            return 0;
        } else {
            if (str_len > 0 && buffer[str_len - 1] == '\n') {
                buffer[str_len - 1] = '\0';
            }
            sscanf(buffer, "%zu %u", &command, &value);
        }

        switch(command) {
            case 1:
                if ((error = SwagPush(&swag0, value)) != NOERRS) {
                    SwagDump(&swag0, error);
                } else {
                printf("\nPushed %d\n\n", value);
                }
            break;

            case 2:
                if ((error = SwagPop(&swag0, &popped1)) != NOERRS) {
                    SwagDump(&swag0, error);
                } else {
                    printf("\nPopped: %d\n\n", popped1);
                }
            break;

            case 3:
                if ((error = SwagSumn(&swag0)) != NOERRS) {
                    printf("Please be carefully, SUMN err\n");
                } else {
                    printf("\nSummed\n\n");
                }
                break;

            case 4:
                if ((error = SwagSub(&swag0)) != NOERRS) {
                    printf("Please be carefully, SUB err\n");
                } else {
                    printf("\nAdded\n\n");
                }
                break;

            case 5:
                if ((error = SwagMul(&swag0)) != NOERRS) {
                    printf("Please be carefully, MUL err\n");
                } else {
                    printf("\nMultiplied\n\n");
                }
                break;

            case 6:
                if ((error = SwagDiv(&swag0)) != NOERRS) {
                    printf("Please be carefully, DIV err\n");
                } else {
                    printf("\nDerivated\n\n");
                }
                break;

            case 7:
                error = SwagVerify(&swag0);
                SwagDump(&swag0, error);
                break;
            case 8:
                *FlagOfExitPTR = true;
                break;

            default:
                printf("\nWrong input, try one more time\n\n");
        }
    }
    printf("Exiting...\n");
    return 0;
}

