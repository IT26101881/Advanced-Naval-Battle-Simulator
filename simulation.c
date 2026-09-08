#include <stdio.h>

#include "simulation.h"
#include "battlefield.h"
#include "battleship.h"
#include "escort.h"
#include "physics.h"

#include "simulation.h"

void copyBattlefield(struct Battlefield *source,
                     struct Battlefield *destination)
{
    *destination = *source;
}
