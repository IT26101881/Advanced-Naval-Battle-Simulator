#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "battleship.h"
#include "escort.h"

struct Battlefield
{
    double size;

    struct Battleship battleship;

    struct EscortShip escorts[100];

    int numberOfEscorts;
};

#endif
