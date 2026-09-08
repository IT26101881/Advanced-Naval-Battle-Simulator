#ifndef SIMULATION_H
#define SIMULATION_H

#include "battlefield.h"

void runBattleSimulation(struct Battlefield *battlefield);

void copyBattlefield(struct Battlefield *source,
                     struct Battlefield *destination);

#endif
