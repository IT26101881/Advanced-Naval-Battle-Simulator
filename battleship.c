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
                     struct EscortShip *escort,
                     double firingTime)
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
        distance,
        battleship->minAngle
    );

    if (firingAngle == -1)
    {
        return 0;
    }

    flightTime = calculateFlightTime(
        battleship->maxVelocity,
        firingAngle
    );

    /*
     * Store the time when the projectile
     * will hit the Escort Ship.
     */
    escort->hitTime = firingTime + flightTime;

    printf("Time to hit: %.2f seconds\n",
           escort->hitTime);

    return 1;
}

void sortEscortsByDistance(struct Battleship *battleship,
                           struct EscortShip escorts[],
                           int numberOfEscorts)
{
    for (int i = 0; i < numberOfEscorts - 1; i++)
    {
        for (int j = i + 1; j < numberOfEscorts; j++)
        {
            double distance1;
            double distance2;

            distance1 = calculateDistance(
                battleship->x,
                battleship->y,
                escorts[i].x,
                escorts[i].y
            );

            distance2 = calculateDistance(
                battleship->x,
                battleship->y,
                escorts[j].x,
                escorts[j].y
            );

            /*
             * Put the closer Escort Ship first.
             */
            if (distance2 < distance1)
            {
                struct EscortShip temporary;

                temporary = escorts[i];
                escorts[i] = escorts[j];
                escorts[j] = temporary;
            }
        }
    }
}

double getNextEscortFireTime(
    struct EscortShip escorts[],
    int numberOfEscorts,
    double currentTime)
{
    double nextTime = -1;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0)
        {
            if (escorts[i].nextFireTime > currentTime)
            {
                if (nextTime == -1 ||
                    escorts[i].nextFireTime < nextTime)
                {
                    nextTime = escorts[i].nextFireTime;
                }
            }
        }
    }

    return nextTime;
}

double getNextEscortHitTime(
    struct EscortShip escorts[],
    int numberOfEscorts,
    double currentTime)
{
    double nextTime = -1;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0 &&
            escorts[i].hitTime > currentTime)
        {
            if (nextTime == -1 ||
                escorts[i].hitTime < nextTime)
            {
                nextTime = escorts[i].hitTime;
            }
        }
    }

    return nextTime;
}

void attackAllEscorts(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts)
{
    double currentTime = 0;

    int remainingEscorts = 0;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0)
        {
            remainingEscorts++;
        }
    }

    if (remainingEscorts == 0)
    {
        printf("All Escort Ships have already been destroyed.\n");
        return;
    }

    /*
     * Arrange Escort Ships from closest
     * to farthest from the Battleship.
     */
    sortEscortsByDistance(
        battleship,
        escorts,
        numberOfEscorts
    );

    printf("\nAttack order: ");

    int first = 1;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0 &&
            battleshipCanReach(
                battleship,
                &escorts[i]) == 1)
        {
            if (first == 0)
            {
                printf(" -> ");
            }

            printf("E%d", escorts[i].id);

            first = 0;
        }
    }

    printf("\n\n");

    /*
     * Fire at each Escort Ship.
     *
     * Each Battleship shot is separated by
     * the Battleship firing interval.
     */
    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0)
        {
            if (battleshipCanReach(
                    battleship,
                    &escorts[i]) == 1)
            {
                printf("Battleship is firing at Escort Ship %d...\n",
                       escorts[i].id);

                if (battleshipAttack(
                        battleship,
                        &escorts[i],
                        currentTime) == 1)
                {
                    printf("Projectile fired at Escort Ship %d.\n",
                           escorts[i].id);

                    currentTime =
                        currentTime +
                        battleship->firingInterval;
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

void processEscortHits(
    struct EscortShip escorts[],
    int numberOfEscorts,
    double battleTime
    )
{
    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0 &&
            escorts[i].hitTime > 0 &&
            battleTime >= escorts[i].hitTime)
        {
            escorts[i].destroyed = 1;

            printf("Escort Ship %d was destroyed at %.2f seconds.\n",
                   escorts[i].id,
                   escorts[i].hitTime);
        }
    }
}
