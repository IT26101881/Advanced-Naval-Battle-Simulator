#ifndef ESCORT_H
#define ESCORT_H

struct EscortShip
{
    int id;
    char type[3];

    double x;
    double y;

    double minVelocity;
    double maxVelocity;
    double velocity;

    double minAngle;
    double maxAngle;
    double angle;

    double impactPower;
    double gamma;

    double hitTime;

    int destroyed;
};

void generateEscortType(struct EscortShip *escort);

void setEscortProperties(struct EscortShip *escort,
                         double battleshipMaxVelocity);

void generateEscortPosition(struct EscortShip *escort,
                            double battlefieldSize);

struct Battleship;

int escortCanReachBattleship(struct EscortShip *escort,
                             struct Battleship *battleship);

int escortAttack(struct EscortShip *escort,
                 struct Battleship *battleship);

void attackBattleship(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts,
                      int wasDestroyed[]);

int countDestroyedEscorts(struct EscortShip escorts[],
                          int numberOfEscorts);

#endif
