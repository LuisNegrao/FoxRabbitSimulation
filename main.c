#include <stdio.h>
#include "Engine.h"
#include <time.h>


int main() {

    FILE *f = stdin;
    bigBang(f);

    printBoard(world->board);

    clock_t init = clock();

    while (world->CURRENT_GENERATION < world->N_GEN) {

        evolve();

        switchBoards();

        world->CURRENT_GENERATION ++;
        //printf("NGEN: %d\n", world->CURRENT_GENERATION);
    }

    clock_t end = clock();

    printBoard(world->board);

    printf("time: %f\n", (float )(end - init));

    return 0;
}
