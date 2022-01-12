#include <stdio.h>
#include "Engine.h"

int main() {

    FILE *f = stdin;
    bigBang(f);

    printBoard(world->board);

    while (world->CURRENT_GENERATION < 1000) {

        evolve();

        switchBoards();

        printBoard(world->board);

        world->CURRENT_GENERATION ++;
        printf("NGEN: %d\n", world->CURRENT_GENERATION);
    }

    return 0;
}
