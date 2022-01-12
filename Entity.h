//
// Created by luisnegrao on 1/11/22.
//

#ifndef FOXANDRABIIT_ENTITY_H
#define FOXANDRABIIT_ENTITY_H

typedef struct Entity Entity;

typedef struct Move {

    int xPos;
    int yPos;
    Entity *entity;

}Move;

typedef struct Movement {

    struct Move* moves;
    struct Move* meals;

    int nMeals;
    int nMoves;

}Movement;

typedef struct Entity {

    enum {
        NONE,
        ROCK,
        FOX,
        RABBIT
    }kind;

    Movement movement;

    int xPos;
    int yPos;
    int procriationTimer;
    int starvingTimer;

}Entity;

Move createMove(int xPos, int yPos, Entity* entity);

Movement createMovement(Entity *entity);

Entity generateChild(Entity *entity);

void printMovement(Entity *entity);

void printEntity(Entity *entity);


#endif //FOXANDRABIIT_ENTITY_H
