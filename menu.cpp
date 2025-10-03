#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "swags.h"





void initialization(swag_t* refSwag) {

    size_t enteredSize = 0;
    swagErr_t errorInit = NOERRS;
    printf("HOLA!\nEnter capacity of your stack\n");
    if (scanf("%zd", &enteredSize) != 1) {
        printf("Wrong input\n");
        return;
    } else {
        char ch = getchar();
        if ((errorInit = SwagInit(refSwag, enteredSize)) == NOERRS) {
            printf("Created new SWAG with capacity of %zu\n", enteredSize);
        } else {
            printf("Initialization error\n");
            SwagDump(refSwag, errorInit);
            return;
        }
    }
    SwagInit(refSwag, enteredSize);
}

void mainCommands(swag_t* refSwag) {
    bool FlagOfExit = false;
    bool* FlagOfExitPTR = &FlagOfExit;
    char* buffer = NULL;
    size_t bufferSize = 0;


    while(!FlagOfExit) {
        printf("Commands:\n1 - PUSH 2 - POP 3 - SUM\n4 - SUB 5 - MUL 6- DIV\n7 - DUMP\n8 - HLT\n");
        printf("Enter in format: COMMAND(num) - value(any int if w/0 arguments)\n\n");


        size_t command = 0;
        swagElem_t value, popped1, popped2 = SWAGVIPERR;

        swagErr_t error = NOERRS;
        int str_len = 0;

        if((str_len = (int)getline(&buffer, &bufferSize, stdin)) == -1) {
            printf("Wrong input\n");
            return;
        } else {
            if (str_len > 0 && buffer[str_len - 1] == '\n') {
                buffer[str_len - 1] = '\0';
            }
            sscanf(buffer, "%zu %u", &command, &value);
        }



        switch(command) {
            case 1:
                if ((error = SwagPush(refSwag, value)) != NOERRS) {
                    SwagDump(refSwag, error);
                } else {
                printf("\nPushed %d\n\n", value);
                }
            break;

            case 2:
                if ((error = SwagPop(refSwag, &popped1)) != NOERRS) {
                    SwagDump(refSwag, error);
                } else {
                    printf("\nPopped: %d\n\n", popped1);
                }
            break;

            case 3:
                if ((error = SwagSumn(refSwag)) != NOERRS) {
                    printf("Please be carefully, SUMN err\n");
                } else {
                    printf("\nSummed\n\n");
                }
                break;

            case 4:
                if ((error = SwagSub(refSwag)) != NOERRS) {
                    printf("Please be carefully, SUB err\n");
                } else {
                    printf("\nAdded\n\n");
                }
                break;

            case 5:
                if ((error = SwagMul(refSwag)) != NOERRS) {
                    printf("Please be carefully, MUL err\n");
                } else {
                    printf("\nMultiplied\n\n");
                }
                break;

            case 6:
                if ((error = SwagDiv(refSwag)) != NOERRS) {
                    printf("Please be carefully, DIV err\n");
                } else {
                    printf("\nDerivated\n\n");
                }
                break;

            case 7:
                error = SwagVerify(refSwag);
                SwagDump(refSwag, error);
                break;
            case 8:
                *FlagOfExitPTR = true;
                break;

            default:
                printf("\nWrong input, try one more time\n\n");
        }
    }
    printf("Exiting...\n");
}
