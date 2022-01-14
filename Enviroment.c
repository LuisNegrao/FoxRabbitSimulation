//
// Created by luisnegrao on 1/11/22.
//

#include "Enviroment.h"
#include "math.h"

void bigBang(FILE *input) {

    int NUMBER;

    world = (World *) malloc(sizeof(World));

    fscanf(input, "%d%d%d%d%d%d%d",
           &world->GEN_RABBIT, &world->GEN_FOX, &world->FOOD_FOX, &world->N_GEN, &world->rows, &world->cols, &NUMBER);

    world->CURRENT_GENERATION = 0;

    world->board = (Entity *) malloc(sizeof(Entity) * world->cols * world->rows);
    world->nextBoard = (Entity *) malloc(sizeof(Entity) * world->cols * world->rows);

    int chunk = (int) ceil((double) world->rows / 16);
#pragma omp parallel for num_threads(1) schedule(static, chunk)
    for (int i = 0; i < world->rows; i++) {
        for (int j = 0; j < world->cols; j++) {
            world->board[CONVERT(world->cols, i, j)].xPos = i;
            world->board[CONVERT(world->cols, i, j)].yPos = j;
            world->nextBoard[CONVERT(world->cols, i, j)].xPos = i;
            world->nextBoard[CONVERT(world->cols, i, j)].yPos = j;
            world->nextBoard[CONVERT(world->cols, i, j)].kind = NONE;
            world->board[CONVERT(world->cols, i, j)].kind = NONE;
        }
    }



    for (int i = 0; i < NUMBER; i++) {

        Entity entity;
        char *kind = (char *) malloc(sizeof(char) * 10);

        fscanf(input, "%s", kind);
        fscanf(input, "%d%d", &entity.xPos, &entity.yPos);
        entity.procriationTimer = 0;
        entity.starvingTimer = 0;

        if (strcmp(kind, "ROCK") == 0) {
            entity.kind = ROCK;
            world->nextBoard[CONVERT(world->cols, entity.xPos, entity.yPos)] = entity;
        } else if (strcmp(kind, "RABBIT") == 0) {
            entity.kind = RABBIT;
        } else if (strcmp(kind, "FOX") == 0) {
            entity.kind = FOX;
        }

        world->board[CONVERT(world->cols, entity.xPos, entity.yPos)] = entity;

    }

    world->locks = (omp_lock_t *) malloc(sizeof(omp_lock_t) * world->rows * world->rows);

    for (int i = 0; i < world->rows * world->rows; i++) {

        omp_init_lock(&world->locks[i]);

    }
}

void printBoard(Entity *board) {
    printf("===========\n");
    for (int i = 0; i < world->rows; ++i) {
        for (int j = 0; j < world->cols; ++j) {

            Entity entity = board[CONVERT(world->cols, i,j)];

            switch (entity.kind) {

                case ROCK:
                    printf("*");
                    break;
                case RABBIT:
                    printf("R");
                    break;
                case FOX:
                    printf("F");
                    break;
                default:
                    printf("-");
                    break;
                
            }
        }
        printf("    ");
        for (int j = 0; j < world->cols; ++j) {

            Entity entity = board[CONVERT(world->cols, i,j)];

            switch (entity.kind) {

                case ROCK:
                    printf("*");
                    break;
                case RABBIT:
                    printf("%d", entity.procriationTimer);
                    break;
                case FOX:
                    printf("%d", entity.procriationTimer);
                    break;
                default:
                    printf("-");
                    break;

            }
        }
        printf("    ");
        for (int j = 0; j < world->cols; ++j) {

            Entity entity = board[CONVERT(world->cols, i,j)];

            switch (entity.kind) {

                case ROCK:
                    printf("*");
                    break;
                case RABBIT:
                    printf("R");
                    break;
                case FOX:
                    printf("%d", entity.starvingTimer);
                    break;
                default:
                    printf("-");
                    break;

            }
        }
        printf("\n");
    }
    printf("==========\n");
}

void switchBoards() {

    world->board = world->nextBoard;
    world->nextBoard = (Entity*) malloc(sizeof (Entity) * world->rows * world->cols);

    int chunk = (int) ceil((double) world->rows / 16);

#pragma omp parallel for num_threads(16) schedule(static, chunk)
    for (int i = 0; i < world->rows; i++) {
        for (int j = 0; j < world->cols; j++) {

            if (world->board[CONVERT(world->cols, i, j)].kind == ROCK) {
                world->nextBoard[CONVERT(world->cols, i, j)].kind = ROCK;
            } else {
                world->nextBoard[CONVERT(world->cols, i, j)].kind = NONE;
            }
            world->nextBoard[CONVERT(world->cols, i, j)].xPos = i;
            world->nextBoard[CONVERT(world->cols, i, j)].yPos = j;
        }
    }

}