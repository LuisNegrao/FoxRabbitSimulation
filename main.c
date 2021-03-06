#include <omp.h>
#include <stdio.h>
#include "Engine.h"
#include "time.h"


int main(int argc, char *argv[]) {


    FILE *f = stdin;
    bigBang(f);

    //printBoard(world->board);


    double init = omp_get_wtime();

    while (world->CURRENT_GENERATION < world->N_GEN) {

        evolve();

        switchBoards();

        world->CURRENT_GENERATION++;
    }

    double end = omp_get_wtime();

    printBoard(world->board);


    printf("time: %f\n", (end - init));

    return 0;
}
