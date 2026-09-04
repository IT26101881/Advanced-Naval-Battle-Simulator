#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.141592653589793

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

struct Battleship
{
    char type;

    double x;
    double y;

    double maxVelocity;

    double gamma;

    double damage;

    int destroyed;
};

struct Battlefield
{
    double size;

    struct Battleship battleship;

    struct EscortShip escorts[100];

    int numberOfEscorts;
};

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
    else if (escort->type[1] == 'E')
    {
        escort->impactPower = 0.04;
        escort->minAngle = 70;

        escort->minVelocity = 0;
        escort->maxVelocity = 0.6 * battleshipMaxVelocity;
    }

    escort->maxAngle = 90;

    escort->velocity =
        generateRandomValue(escort->minVelocity,
                            escort->maxVelocity);

    escort->angle =
        generateRandomValue(escort->minAngle,
                            escort->maxAngle);

    escort->destroyed = 0;
    escort->hitTime = 0;
}

void generateEscortPosition(struct EscortShip *escort,
                            double battlefieldSize)
{
    escort->x =
        ((double)rand() / RAND_MAX) * battlefieldSize;

    escort->y =
        ((double)rand() / RAND_MAX) * battlefieldSize;
}

double calculateRange(double velocity, double angle)
{
    double gravity = 9.81;
    double angleRadians;
    double range;

    angleRadians = angle * PI / 180.0;

    range = (velocity * velocity *
             sin(2 * angleRadians)) / gravity;

    return range;
}

double calculateFiringAngle(double velocity, double distance)
{
    double gravity = 9.81;
    double value;
    double angleRadians;
    double angleDegrees;

    value = (distance * gravity) /
            (velocity * velocity);

    if (value > 1)
    {
        return -1;
    }

    angleRadians = asin(value) / 2.0;

    angleDegrees = angleRadians * 180.0 / PI;

    return angleDegrees;
}

double calculateFlightTime(double velocity, double angle)
{
    double gravity = 9.81;
    double angleRadians;
    double time;

    angleRadians = angle * PI / 180.0;

    time = (2 * velocity * sin(angleRadians)) / gravity;

    return time;
}

double calculateDistance(double x1, double y1, double x2, double y2)
{
    double distance;

    distance = sqrt((x2 - x1) * (x2 - x1) +
                    (y2 - y1) * (y2 - y1));

    return distance;
}

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
    int hitCount = 0;

    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0)
        {
            if (battleshipAttack(battleship, &escorts[i]) == 1)
            {
                hitCount++;

                printf("Battleship hit Escort Ship %d.\n",
                       escorts[i].id);
            }
            else
            {
                printf("Battleship missed Escort Ship %d.\n",
                       escorts[i].id);
            }
        }
    }

    printf("\nTotal Escort Ships hit: %d\n", hitCount);
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
        battleship->destroyed = 1;
        return 1;
    }

    return 0;
}

void attackBattleship(struct Battleship *battleship,
                      struct EscortShip escorts[],
                      int numberOfEscorts)
{
    for (int i = 0; i < numberOfEscorts; i++)
    {
        if (escorts[i].destroyed == 0)
        {
            if (escortAttack(&escorts[i], battleship) == 1)
            {
                printf("Escort Ship %d hit the Battleship.\n",
                       escorts[i].id);

                printf("Battleship has been destroyed!\n");

                return;
            }
            else
            {
                printf("Escort Ship %d missed the Battleship.\n",
                       escorts[i].id);
            }
        }
    }

    printf("Battleship survived all Escort attacks.\n");
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

void runBattleSimulation(struct Battlefield *battlefield)
{
    //double battleTime = 0.0;

    printf("\n====================================\n");
    printf("        BATTLE SIMULATION\n");
    printf("====================================\n");

    printf("\n--- Battleship Attack ---\n");

    attackAllEscorts(
        &battlefield->battleship,
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    int destroyedCount;

    destroyedCount = countDestroyedEscorts(
    battlefield->escorts,
    battlefield->numberOfEscorts
);

printf("\nEscort Ships destroyed: %d\n", destroyedCount);

    if (battlefield->battleship.destroyed == 1)
    {
        printf("\nBattleship was destroyed.\n");
        return;
    }

    printf("\n--- Escort Attack ---\n");

    attackBattleship(
        &battlefield->battleship,
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    if (battlefield->battleship.destroyed == 1)
    {
        printf("\nResult: Battleship was destroyed.\n");
    }
    else
    {
        printf("\nResult: Battleship survived.\n");
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

    runBattleSimulation(&battlefield);

    return 0;
}
