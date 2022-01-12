//
// Created by luisnegrao on 1/11/22.
//

#ifndef FOXANDRABIIT_ENVIROMENT_H
#define FOXANDRABIIT_ENVIROMENT_H

#include "Entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CONVERT(size, row, col) (((row) * (size)) + (col))

typedef struct World {

    int cols;
    int rows;
    int GEN_FOX;
    int GEN_RABBIT;
    int FOOD_FOX;
    int N_GEN;
    int CURRENT_GENERATION;

    Entity *board;
    Entity *nextBoard;

}World;

extern struct World *world;


/**
 * Starts the world with the give config
 */
void bigBang(FILE *input);
void printBoard(Entity *board);
void switchBoards();
#endif //FOXANDRABIIT_ENVIROMENT_H
