//
// Creted by luisnegrao on 1/11/22.
//
#include "Engine.h"
#include "omp.h"
#include "math.h"

int isStronger(Entity *challanger, Entity *reiciver) {

    if (challanger->kind != reiciver->kind) {

        return 1;
    }

    if (challanger->procriationTimer > reiciver->procriationTimer) {
        return 1;
    }

    if (challanger->kind == FOX && reiciver->kind == FOX) {

        if (challanger->procriationTimer == reiciver->procriationTimer &&
            challanger->starvingTimer < reiciver->starvingTimer) {
            return 1;
        }

    }

    return 0;
}

void move(Entity *entity) {
    //printf("ready to move\n");
    int numberOfMoves = entity->movement.nMoves;

    if (numberOfMoves == 0) {

        omp_set_lock(&world->locks[CONVERT(world->cols, entity->xPos, entity->yPos)]);

        Entity *destiny = &world->nextBoard[CONVERT(world->cols, entity->xPos, entity->yPos)];

        if (isStronger(entity, destiny)) {
            world->nextBoard[CONVERT(world->cols, entity->xPos, entity->yPos)] = *entity;
        }
        omp_unset_lock(&world->locks[CONVERT(world->cols, entity->xPos, entity->yPos)]);

        return;
    }

    int index;

    if (numberOfMoves == 1) {

        index = 0;

    } else if (numberOfMoves > 1) {

        index = (world->CURRENT_GENERATION + entity->xPos + entity->yPos) % numberOfMoves;
        //printf("index: %d", index);
    }
    //printMovement(entity);

    Entity *destiny = &world->nextBoard[CONVERT(world->cols, entity->movement.moves[index].xPos,
                                               entity->movement.moves[index].yPos)];

    if (entity->movement.moves[index].type == WEST || entity->movement.moves[index].type == EAST) {
        if (isStronger(entity, destiny)) {

            entity->xPos = destiny->xPos;
            entity->yPos = destiny->yPos;
            world->nextBoard[CONVERT(world->cols, entity->xPos, entity->yPos)] = *entity;

        }
        return;
    }

    omp_set_lock(&world->locks[CONVERT(world->cols, entity->movement.moves[index].xPos,
                                       entity->movement.moves[index].yPos)]);

    if (isStronger(entity, destiny)) {

        entity->xPos = destiny->xPos;
        entity->yPos = destiny->yPos;
        world->nextBoard[CONVERT(world->cols, entity->xPos, entity->yPos)] = *entity;

    }

    omp_unset_lock(&world->locks[CONVERT(world->cols, entity->movement.moves[index].xPos,
                                       entity->movement.moves[index].yPos)]);


}

void eat(Entity *entity) {

    int numberOfMeals = entity->movement.nMeals;

    if (numberOfMeals == 0) return;

    int index;

    if (numberOfMeals == 1) {

        index = 0;

    }
    if (numberOfMeals > 1) {

        index = (world->CURRENT_GENERATION + entity->xPos + entity->yPos) % numberOfMeals;

    }

    Entity *destiny = &world->nextBoard[CONVERT(world->cols, entity->movement.meals[index].xPos,
                                                entity->movement.meals[index].yPos)];

    if (entity->movement.meals[index].type == WEST || entity->movement.meals[index].type == EAST) {
        if (isStronger(entity, destiny)) {

            entity->xPos = destiny->xPos;
            entity->yPos = destiny->yPos;
            world->nextBoard[CONVERT(world->cols, entity->xPos, entity->yPos)] = *entity;

        }
        return;
    }


    omp_set_lock(&world->locks[CONVERT(world->cols, entity->movement.meals[index].xPos,
                                       entity->movement.meals[index].yPos)]);

    entity->xPos = destiny->xPos;
    entity->yPos = destiny->yPos;

    world->nextBoard[CONVERT(world->cols, entity->xPos, entity->yPos)] = *entity;

    omp_unset_lock(&world->locks[CONVERT(world->cols, entity->movement.meals[index].xPos,
                                       entity->movement.meals[index].yPos)]);


}

void evolveRabbit(Entity *entity) {
    entity->procriationTimer++;

    if (entity->procriationTimer > world->GEN_RABBIT) {

        if (entity->movement.nMoves > 0) {
            Entity child = generateChild(entity);
            if (isStronger(&child, &world->nextBoard[CONVERT(world->cols, child.xPos, child.yPos)])) {

                world->nextBoard[CONVERT(world->cols, child.xPos, child.yPos)] = child;

            }
            entity->procriationTimer = 0;
        }
        move(entity);
    } else {

        move(entity);

    }

}

void evolveFox(Entity *entity) {

    entity->procriationTimer++;
    entity->starvingTimer++;

    //printf("%d\n", entity->movement.nMeals);

    if (entity->movement.nMeals >= 1) {

        if (entity->procriationTimer > world->GEN_FOX) {

            Entity child = generateChild(entity);
            if (isStronger(&child, &world->nextBoard[CONVERT(world->cols, child.xPos, child.yPos)])) {

                world->nextBoard[CONVERT(world->cols, child.xPos, child.yPos)] = child;

            }

            entity->procriationTimer = 0;
        }
        entity->starvingTimer = 0;
        eat(entity);

    } else if (entity->starvingTimer == world->FOOD_FOX) {
        //printf("should die x: %d, y: %d\n", entity->xPos, entity->yPos);
        //TODO DEAD

    } else if (entity->procriationTimer > world->GEN_FOX) {

        if (entity->movement.nMoves > 0) {

            Entity child = generateChild(entity);
            if (isStronger(&child, &world->nextBoard[CONVERT(world->cols, child.xPos, child.yPos)])) {

                world->nextBoard[CONVERT(world->cols, child.xPos, child.yPos)] = child;

            }
            entity->procriationTimer = 0;

        }
        move(entity);

    } else {

        move(entity);
    }

}

void evolve() {
    {

        int amount = world->cols;

        for (int k = 0; k < 4; ++k) {
            #pragma omp parallel for num_threads(16) schedule(static, 20) collapse(2)
            for (int i = 0; i < world->rows; i++) {
                for (int j = 0; j < world->cols; ++j) {
                    //printf("x: %d, y: %d %d\n",i,j, omp_get_thread_num());
                    Entity *entity = &world->board[CONVERT(world->cols, i, j)];

                    if (entity->kind == RABBIT && k == 0) {
                        entity->movement = createMovement(entity);
                    }

                    if (entity->kind == RABBIT && k == 1) {

                        evolveRabbit(entity);
                    }


                    if (entity->kind == FOX && k == 2) {
                        entity->movement = createMovement(entity);
                    }

                    if (entity->kind == FOX && k == 3) {
                        evolveFox(entity);
                    }


                }
            }
        }
    }

}