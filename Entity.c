//
// Created by luisnegrao on 1/11/22.
//

#include "Enviroment.h"
#include <stdlib.h>

struct World *world;

Move createMove(int xPos, int yPos, Entity *entity) {

    Move move;
    move.xPos = xPos;
    move.yPos = yPos;
    move.entity = entity;
    return move;
}

Movement createMovement(Entity *entity) {

    int moves[4][2] = {{entity->xPos - 1, entity->yPos},
                       {entity->xPos,     entity->yPos + 1},
                       {entity->xPos + 1, entity->yPos},
                       {entity->xPos,     entity->yPos - 1}};

    Movement movement;
    movement.moves = (Move *) malloc(sizeof(Move) * 4);
    movement.meals = (Move *) malloc(sizeof(Move) * 4);
    movement.nMoves = 0;
    movement.nMeals = 0;

    int moveIndex = 0;
    int mealIndex = 0;

    for (int i = 0; i < 4; i++) {

        if (moves[i][0] < world->rows && moves[i][1] < world->cols &&
            moves[i][0] >= 0 && moves[i][1] >= 0) {

            Entity *target = &world->board[CONVERT(world->cols, moves[i][0], moves[i][1])];
            Entity *nextBoardTarget = &world->nextBoard[CONVERT(world->cols, moves[i][0], moves[i][1])];

            if (entity->kind == FOX) {

                if (target->kind == NONE || target->kind == RABBIT) {

                    Move move = createMove(moves[i][0], moves[i][1], nextBoardTarget);
                    movement.moves[moveIndex] = move;
                    movement.nMoves++;
                    moveIndex++;

                }
                if (nextBoardTarget->kind == RABBIT) {

                    Move move = createMove(moves[i][0], moves[i][1], nextBoardTarget);
                    movement.meals[mealIndex] = move;
                    movement.nMeals++;
                    mealIndex++;

                }

            } else if (target->kind == NONE) {
                Move move = createMove(moves[i][0], moves[i][1], target);
                movement.nMoves++;
                movement.moves[moveIndex] = move;
                moveIndex++;
            }
        }
    }
    return movement;
}

Entity generateChild(Entity *entity) {

    Entity child;
    child.xPos = entity->xPos;
    child.yPos = entity->yPos;
    child.kind = entity->kind;
    child.procriationTimer = 0;
    child.starvingTimer = 0;

    return child;

}

void printMovement(Entity *entity) {

    printf("Moves\n");
    for (int i = 0; i < entity->movement.nMoves; i++) {
        printf("x: %d y: %d\n", entity->movement.moves[i].xPos, entity->movement.moves[i].yPos);
    }
    printf("Meals\n");

}

void printEntity(Entity *entity) {
    printf("Entity\n");
    printf("x: %d, y: %d\n", entity->xPos, entity->yPos);
}