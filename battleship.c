#include <stdio.h>

#include "battleship.h"
#include "physics.h"

int battleshipCanReach(struct Battleship *battleship,
                       struct EscortShip *escort)
{
    double distance;
    double maximumRange;

    distance = calculateDistance(
        battleship->x,
        battleship->y,
        escort->x,
        escort->y
    );

    maximumRange = calculateRange(
        battleship->maxVelocity,
        45
    );

    if (distance <= maximumRange)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int battleshipAttack(struct Battleship *battleship,
                     struct EscortShip *escort)
{
    double distance;
    double firingAngle;
    double flightTime;

    distance = calculateDistance(
        battleship->x,
        battleship->y,
        escort->x,
        escort->y
    );

    firingAngle = calculateFiringAngle(
        battleship->maxVelocity,
        distance
    );

    if (firingAngle == -1)
    {
        return 0;
    }

    flightTime = calculateFlightTime(
        battleship->maxVelocity,
        firingAngle
    );

    escort->hitTime = flightTime;
    escort->destroyed = 1;

    printf("Time to hit: %.2f seconds\n",
           flightTime);

    return 1;
}

void attackAllEscorts(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts)
{
    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0)
        {
            if (battleshipCanReach(battleship, &escorts[i]) == 1)
            {
                if (battleshipAttack(battleship, &escorts[i]) == 1)
                {
                    printf("Battleship hit Escort Ship %d.\n",
                           escorts[i].id);
                }
            }
            else
            {
                printf("Escort Ship %d is out of range.\n",
                       escorts[i].id);
            }
        }
    }
}


