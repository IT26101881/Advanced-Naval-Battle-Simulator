#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct EscortShip
{
    int id;
    char type[3];

    double x;
    double y;

    double minVelocity;
    double maxVelocity;

    double minAngle;
    double maxAngle;

    double impactPower;
    double gamma;

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

void setEscortProperties(struct EscortShip *escort)
{
    if (escort->type[1] == 'A')
    {
        escort->impactPower = 0.08;
        escort->minAngle = 20;
    }
    else if (escort->type[1] == 'B')
    {
        escort->impactPower = 0.06;
        escort->minAngle = 30;
    }
    else if (escort->type[1] == 'C')
    {
        escort->impactPower = 0.07;
        escort->minAngle = 25;
    }
    else if (escort->type[1] == 'D')
    {
        escort->impactPower = 0.05;
        escort->minAngle = 50;
    }
    else if (escort->type[1] == 'E')
    {
        escort->impactPower = 0.04;
        escort->minAngle = 70;
    }

    escort->maxAngle = 90;
    escort->destroyed = 0;
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

    // Display entered information
    printf("\n===== Battleship Information =====\n");
    printf("Battleship Type: %c\n", battlefield.battleship.type);

    printf("Position: (%.2f, %.2f)\n", battlefield.battleship.x, battlefield.battleship.y);

    printf("Maximum Shell Velocity: %.2f\n", battlefield.battleship.maxVelocity);

    printf("Number of Escort Ships: %d\n", battlefield.numberOfEscorts);

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

    printf("Impact Power: %.2f\n",
           battlefield.escorts[i].impactPower);

    printf("Minimum Angle: %.2f degrees\n",
           battlefield.escorts[i].minAngle);

    printf("Maximum Angle: %.2f degrees\n",
           battlefield.escorts[i].maxAngle);
}

    return 0;
}
