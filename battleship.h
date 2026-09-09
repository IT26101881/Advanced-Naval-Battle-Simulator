#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "escort.h"

struct Battleship
{
    char type;

    double x;
    double y;

    double maxVelocity;

    double firingInterval;

    double minAngle;
    double maxAngle;

    double gamma;

    double damage;

    int destroyed;
};

int battleshipCanReach(struct Battleship *battleship,
                       struct EscortShip *escort);

int battleshipAttack(struct Battleship *battleship,
                     struct EscortShip *escort,
                     double firingTime);

void attackAllEscorts(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts);

void sortEscortsByDistance(struct Battleship *battleship,
                           struct EscortShip escorts[],
                           int numberOfEscorts);

void processEscortHits(
    struct EscortShip escorts[],
    int numberOfEscorts,
    double battleTime
    );

double getNextEscortFireTime(
    struct EscortShip escorts[],
    int numberOfEscorts,
    double currentTime
);

double getNextEscortHitTime(
    struct EscortShip escorts[],
    int numberOfEscorts,
    double currentTime);

#endif
