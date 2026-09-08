#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "escort.h"

struct Battleship
{
    char type;

    double x;
    double y;

    double maxVelocity;

    double minAngle;
    double maxAngle;

    double gamma;

    double damage;

    int destroyed;
};

int battleshipCanReach(struct Battleship *battleship,
                       struct EscortShip *escort);

int battleshipAttack(struct Battleship *battleship,
                     struct EscortShip *escort);

void attackAllEscorts(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts);

#endif
