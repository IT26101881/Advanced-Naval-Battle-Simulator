#include <stdlib.h>
#include <stdio.h>

#include "escort.h"
#include "battleship.h"
#include "physics.h"

double generateRandomValue(double minimum, double maximum)
{
    return minimum +
           ((double)rand() / RAND_MAX) *
           (maximum - minimum);
}

void generateEscortType(struct EscortShip *escort)
{
    int typeNumber;

    typeNumber = rand() % 5;

    if (typeNumber == 0)
    {
        escort->type[0] = 'E';
        escort->type[1] = 'A';
    }
    else if (typeNumber == 1)
    {
        escort->type[0] = 'E';
        escort->type[1] = 'B';
    }
    else if (typeNumber == 2)
    {
        escort->type[0] = 'E';
        escort->type[1] = 'C';
    }
    else if (typeNumber == 3)
    {
        escort->type[0] = 'E';
        escort->type[1] = 'D';
    }
    else
    {
        escort->type[0] = 'E';
        escort->type[1] = 'E';
    }

    escort->type[2] = '\0';
}

void setEscortProperties(struct EscortShip *escort,
                         double battleshipMaxVelocity)
{
    if (escort->type[1] == 'A')
    {
        escort->impactPower = 0.08;
        escort->minAngle = 20;

        escort->minVelocity = 0;
        escort->maxVelocity = 1.2 * battleshipMaxVelocity;
    }
    else if (escort->type[1] == 'B')
    {
        escort->impactPower = 0.06;
        escort->minAngle = 30;

        escort->minVelocity = 0;
        escort->maxVelocity = 0.9 * battleshipMaxVelocity;
    }
    else if (escort->type[1] == 'C')
    {
        escort->impactPower = 0.07;
        escort->minAngle = 25;

        escort->minVelocity = 0;
        escort->maxVelocity = 0.8 * battleshipMaxVelocity;
    }
    else if (escort->type[1] == 'D')
    {
        escort->impactPower = 0.05;
        escort->minAngle = 50;

        escort->minVelocity = 0;
        escort->maxVelocity = 0.7 * battleshipMaxVelocity;
    }
    else
    {
        escort->impactPower = 0.04;
        escort->minAngle = 70;

        escort->minVelocity = 0;
        escort->maxVelocity = 0.6 * battleshipMaxVelocity;
    }

    escort->maxAngle = 90;

    escort->velocity =
        generateRandomValue(
            escort->minVelocity,
            escort->maxVelocity
        );

    escort->angle =
        generateRandomValue(
            escort->minAngle,
            escort->maxAngle
        );

    escort->destroyed = 0;
    escort->hitTime = 0;
}

void generateEscortPosition(struct EscortShip *escort,
                            double battlefieldSize)
{
    escort->x =
        generateRandomValue(0, battlefieldSize);

    escort->y =
        generateRandomValue(0, battlefieldSize);
}

int escortCanReachBattleship(struct EscortShip *escort,
                             struct Battleship *battleship)
{
    double distance;
    double range;

    distance = calculateDistance(
        escort->x,
        escort->y,
        battleship->x,
        battleship->y
    );

    range = calculateRange(
        escort->velocity,
        escort->angle
    );

    if (distance <= range)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int escortAttack(struct EscortShip *escort,
                 struct Battleship *battleship)
{
    if (escortCanReachBattleship(escort, battleship) == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void attackBattleship(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts,
                      int wasDestroyed[])
{
    int hitCount = 0;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        /*
         * Only Escort Ships that were alive at the
         * beginning of this battle position and
         * have not fired yet can fire.
         */
        if (wasDestroyed[i] == 0 &&
            escorts[i].hasFired == 0)
        {
            if (escortAttack(&escorts[i], battleship) == 1)
            {
                hitCount++;

                battleship->damage =
                    battleship->damage +
                    escorts[i].impactPower;

                printf("Escort Ship %d hit the Battleship.\n",
                       escorts[i].id);

                printf("Damage caused: %.2f%%\n",
                       escorts[i].impactPower * 100);

                printf("Battleship total damage: %.2f%%\n",
                       battleship->damage * 100);

                printf("Battleship health: %.2f%%\n",
                       (1.0 - battleship->damage) * 100);

                escorts[i].hasFired = 1;

                if (battleship->damage >= 1.0)
                {
                    battleship->destroyed = 1;

                    printf("\nBattleship was destroyed by accumulated damage.\n");

                    return;
                }
            }
            else
            {
                printf("Escort Ship %d missed the Battleship.\n",
                       escorts[i].id);

                escorts[i].hasFired = 1;
            }
        }
    }

    if (hitCount == 0)
    {
        printf("All Escort Ships missed the Battleship.\n");
    }
}

int countDestroyedEscorts(struct EscortShip escorts[],
                          int numberOfEscorts)
{
    int count = 0;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 1)
        {
            count++;
        }
    }

    return count;
}
