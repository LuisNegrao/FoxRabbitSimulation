//
// Created by luisnegrao on 1/11/22.
//

#ifndef FOXANDRABIIT_ENGINE_H
#define FOXANDRABIIT_ENGINE_H
#include "Enviroment.h"

/**
 *
 * Moves a given entity to the correct place
 *
 */
void move(Entity *entity);
void eat(Entity *entity);
void evolve();
void evolveRabbit(Entity *entity);
void evolveFox(Entity *entity);
int isStronger(Entity *challanger, Entity *reiciver);

#endif //FOXANDRABIIT_ENGINE_H
