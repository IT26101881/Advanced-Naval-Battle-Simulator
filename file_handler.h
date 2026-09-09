#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "battlefield.h"

void saveInitialConditions(struct Battlefield *battlefield);

void saveFinalBattlefield(struct Battlefield *battlefield);

void saveSimulation2FinalBattlefield(
    struct Battlefield *battlefield
);

void saveAttackOrder(
    struct Battlefield *battlefield,
    int pointNumber
);

#endif
