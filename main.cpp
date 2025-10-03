#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "swags.h"
#include "menu.h"

int main(void) {

    swag_t swag0 = {};
    initialization(&swag0);
    mainCommands(&swag0);
    return 0;
}

