#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "battleship.h"
#include "physics.h"
#include "escort.h"
#include "battlefield.h"
#include "file_handler.h"

double generateRandomValue(double minimum, double maximum);

#define PI 3.141592653589793

struct MovementPoint
{
    double x;
    double y;
};

int isWithinRange(double velocity, double angle,
                 double x1, double y1,
                 double x2, double y2)
{
    double range;
    double distance;

    range = calculateRange(velocity, angle);

    distance = calculateDistance(x1, y1, x2, y2);

    if (distance <= range)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void generateBattleshipPositions(struct Battlefield *battlefield,
                                 struct MovementPoint points[],
                                 int k)
{
    printf("\n===== BATTLESHIP MOVEMENT POINTS =====\n");

    for (int i = 0; i < k; i++)
    {
        points[i].x =
            generateRandomValue(0, battlefield->size);

        points[i].y =
            generateRandomValue(0, battlefield->size);

        printf("Point %d: (%.2f, %.2f)\n",
               i + 1,
               points[i].x,
               points[i].y);
    }
}

void runSimulationAtPoint(struct Battlefield *battlefield,
                          double x,
                          double y,
                          int pointNumber)
{
    double battleEndTime = 0;
    int wasDestroyed[100];

    printf("\n------------------------------------\n");
    printf("          BATTLE POSITION %d\n", pointNumber);
    printf("------------------------------------\n");

    battlefield->battleship.x = x;
    battlefield->battleship.y = y;

    printf("Battleship Position: (%.2f, %.2f)\n",
           battlefield->battleship.x,
           battlefield->battleship.y);

    /*
     * Remember which Escort Ships were already destroyed
     * before this movement point.
     */
    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        wasDestroyed[i] = battlefield->escorts[i].destroyed;
    }

    printf("\nBattleship is firing...\n\n");

    attackAllEscorts(
        &battlefield->battleship,
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    /*
     * Calculate Battle End Time using only Escort Ships
     * destroyed at this movement point.
     */
    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        if (wasDestroyed[i] == 0 &&
            battlefield->escorts[i].destroyed == 1)
        {
            if (battlefield->escorts[i].hitTime > battleEndTime)
            {
                battleEndTime = battlefield->escorts[i].hitTime;
            }
        }
    }

    if (battleEndTime > 0)
    {
        printf("\nBattle End Time: %.2f seconds\n",
               battleEndTime);
    }
    else
    {
        printf("\nNo Escort Ships were hit at this position.\n");
    }

    if (battlefield->battleship.destroyed == 1)
    {
        printf("\nBattleship was destroyed at Point %d.\n",
               pointNumber);
        return;
    }

    printf("\nEnemy ships are returning fire...\n\n");

    attackBattleship(
        &battlefield->battleship,
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    if (battlefield->battleship.destroyed == 1)
    {
        printf("\nBattleship was destroyed at Point %d.\n",
               pointNumber);
    }
    else
    {
        printf("\nBattleship survived at Point %d.\n",
               pointNumber);
    }
}

int main()
{
    srand(time(NULL));

    struct Battlefield battlefield;


    // Battlefield setup

    battlefield.size = 100;


    // Battleship setup

    printf("===== Battleship Setup =====\n");

    printf("Enter Battleship Type (U/M/R/S): ");
    scanf(" %c", &battlefield.battleship.type);

    printf("Enter Battleship X position: ");
    scanf("%lf", &battlefield.battleship.x);

    printf("Enter Battleship Y position: ");
    scanf("%lf", &battlefield.battleship.y);

    printf("Enter Battleship maximum shell velocity: ");
    scanf("%lf", &battlefield.battleship.maxVelocity);


    // Other initial values

    battlefield.battleship.gamma = 0.001;
    battlefield.battleship.damage = 0;
    battlefield.battleship.destroyed = 0;


    // Number of escort ships

    printf("Enter number of Escort Ships: ");
    scanf("%d", &battlefield.numberOfEscorts);

    // Escort ship setup

    printf("\n===== Escort Ship Setup =====\n");

    for (int i = 0; i < battlefield.numberOfEscorts; i++)
    {
        battlefield.escorts[i].id = i + 1;

        printf("\nEscort Ship %d\n",
               battlefield.escorts[i].id);

        generateEscortType(&battlefield.escorts[i]);

        setEscortProperties(
            &battlefield.escorts[i],
            battlefield.battleship.maxVelocity
        );

        generateEscortPosition(
            &battlefield.escorts[i],
            battlefield.size
        );
    }

    int k;

    printf("\nEnter number of Battleship movement points (k): ");
    scanf("%d", &k);

    struct MovementPoint points[k];

    generateBattleshipPositions(
        &battlefield,
        points,
        k
    );


    // Display battleship information

    printf("\n===== Battleship Information =====\n");

    printf("Battleship Type: %c\n",
           battlefield.battleship.type);

    printf("Position: (%.2f, %.2f)\n",
           battlefield.battleship.x,
           battlefield.battleship.y);

    printf("Maximum Shell Velocity: %.2f\n",
           battlefield.battleship.maxVelocity);

    printf("Number of Escort Ships: %d\n",
           battlefield.numberOfEscorts);


    // Display escort information

    printf("\n===== Escort Ship Information =====\n");

    for (int i = 0; i < battlefield.numberOfEscorts; i++)
    {
        printf("\nEscort ID: %d\n",
               battlefield.escorts[i].id);

        printf("Type: %s\n",
               battlefield.escorts[i].type);

        printf("Position: (%.2f, %.2f)\n",
               battlefield.escorts[i].x,
               battlefield.escorts[i].y);

        printf("Minimum Velocity: %.2f\n",
               battlefield.escorts[i].minVelocity);

        printf("Maximum Velocity: %.2f\n",
               battlefield.escorts[i].maxVelocity);

        printf("Actual Velocity: %.2f\n",
               battlefield.escorts[i].velocity);

        printf("Minimum Angle: %.2f degrees\n",
               battlefield.escorts[i].minAngle);

        printf("Maximum Angle: %.2f degrees\n",
               battlefield.escorts[i].maxAngle);

        printf("Actual Angle: %.2f degrees\n",
               battlefield.escorts[i].angle);

        printf("Impact Power: %.2f\n",
               battlefield.escorts[i].impactPower);
    }

    saveInitialConditions(&battlefield);

    printf("\n\n====================================\n");
    printf("          NAVAL BATTLE\n");
    printf("====================================\n");

    for (int i = 0; i < k; i++)
    {
        if (battlefield.battleship.destroyed == 1)
        {
            printf("\nBattleship has been destroyed.");
            printf("\nSimulation stopped at Point %d.\n", i + 1);
            break;
        }

        runSimulationAtPoint(
            &battlefield,
            points[i].x,
            points[i].y,
            i + 1
        );

        printf("\nEscort Ships destroyed so far: %d\n",
            countDestroyedEscorts(
                battlefield.escorts,
                battlefield.numberOfEscorts
                ));
    }

    saveFinalBattlefield(&battlefield);
   
    return 0;
}
