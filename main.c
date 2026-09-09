#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "battleship.h"
#include "physics.h"
#include "escort.h"
#include "battlefield.h"
#include "file_handler.h"
#include "simulation.h"

double generateRandomValue(double minimum, double maximum);
void startSimulation();

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
                          int pointNumber,
                          double minimumAngle)
{
    double battleTime = 0;
    double battleEndTime = 0;

    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        if (battlefield->escorts[i].destroyed == 0)
        {
            battlefield->escorts[i].hitTime = 0;
            battlefield->escorts[i].nextFireTime =
                battlefield->escorts[i].firingInterval;
        }
    }

    printf("\n------------------------------------\n");
    printf("          BATTLE POSITION %d\n", pointNumber);
    printf("------------------------------------\n");

    battlefield->battleship.x = x;
    battlefield->battleship.y = y;

    battlefield->battleship.minAngle = minimumAngle;

    if (minimumAngle > 0)
    {
        printf("WARNING: Battleship gun is JAMMED!\n");
        printf("Minimum firing angle is now %.2f degrees.\n",
               minimumAngle);
    }

    printf("Battleship Position: (%.2f, %.2f)\n",
           battlefield->battleship.x,
           battlefield->battleship.y);

    printf("\nBattleship is firing...\n\n");

    attackAllEscorts(
        &battlefield->battleship,
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    saveAttackOrder(
        battlefield,
        pointNumber
    );

    int escortsAlive = 0;

    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        if (battlefield->escorts[i].destroyed == 0)
        {
            escortsAlive++;
        }
    }


    if (escortsAlive == 0)
    {

        return;
    }

    /*
     * Find the last Battleship projectile
     * impact time.
     */
    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        if (battlefield->escorts[i].hitTime > battleEndTime)
        {
            battleEndTime =
                battlefield->escorts[i].hitTime;
        }
    }

    printf("\nBattle End Time: %.2f seconds\n",
           battleEndTime);

    printf("\nEnemy ships are returning fire...\n\n");

    /*
     * Process all events in chronological order.
     */
    while (battlefield->battleship.destroyed == 0)
    {
        double nextFireTime =
            getNextEscortFireTime(
                battlefield->escorts,
                battlefield->numberOfEscorts,
                battleTime
            );

        double nextHitTime =
            getNextEscortHitTime(
                battlefield->escorts,
                battlefield->numberOfEscorts,
                battleTime
            );

        double nextEventTime = -1;

        /*
         * Select next Escort firing event.
         */
        if (nextFireTime != -1)
        {
            nextEventTime = nextFireTime;
        }

        /*
         * Select next Battleship projectile impact.
         */
        if (nextHitTime != -1)
        {
            if (nextEventTime == -1 ||
                nextHitTime < nextEventTime)
            {
                nextEventTime = nextHitTime;
            }
        }

        /*
         * No events remain.
         */
        if (nextEventTime == -1)
        {
            break;
        }

        /*
         * Move to the exact event time.
         */
        battleTime = nextEventTime;

        /*
         * Do not process events after the
         * final Battleship projectile impact.
         */
        if (battleTime > battleEndTime)
        {
            break;
        }

        /*
         * First destroy Escorts whose
         * Battleship projectile has arrived.
         */
        processEscortHits(
            battlefield->escorts,
            battlefield->numberOfEscorts,
            battleTime
        );

        /*
         * Then allow surviving Escorts to fire.
         */
        attackBattleship(
            &battlefield->battleship,
            battlefield->escorts,
            battlefield->numberOfEscorts,
            battleTime
        );
    }

    printf("\nBattleship total damage: %.2f%%\n",
           battlefield->battleship.damage * 100);

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

void startSimulation ()
{

    FILE *file = fopen("attack_order.txt", "w");

    if (file != NULL)
    {
        fclose(file);
    }

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

    printf("Enter time between Battleship shots (seconds): ");
    scanf("%lf", &battlefield.battleship.firingInterval);


    // Other initial values

    battlefield.battleship.minAngle = 0;
    battlefield.battleship.maxAngle = 90;

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

        battlefield.escorts[i].nextFireTime = battlefield.escorts[i].firingInterval;

        generateEscortPosition(
            &battlefield.escorts[i],
            battlefield.size
        );
    }

    int k;

    printf("\nEnter number of Battleship movement points (k): ");
    scanf("%d", &k);

    int jamPoint;
    double jamAngle;

    printf("Enter gun jam point (1-%d): ", k);
    scanf("%d", &jamPoint);

    printf("Enter minimum firing angle after jam (0-30): ");
    scanf("%lf", &jamAngle);

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

    printf("Health: %.2f%%\n",
            (1.0 - battlefield.battleship.damage) * 100);

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
               
        printf("Firing Interval: %.2f seconds\n",
               battlefield.escorts[i].firingInterval);
    }

    struct Battlefield simulation2Battlefield;

    copyBattlefield(&battlefield, &simulation2Battlefield);

    saveInitialConditions(&battlefield);

    printf("\n\n====================================\n");
    printf("          NAVAL BATTLE\n");
    printf("====================================\n");

    printf("\n\n====================================\n");
    printf("          SIMULATION 1\n");
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
            i + 1,
            0
        );

        printf("\nEscort Ships destroyed so far: %d\n",
            countDestroyedEscorts(
                battlefield.escorts,
                battlefield.numberOfEscorts
            ));
    }

    printf("\n\n====================================\n");
    printf("          SIMULATION 2\n");
    printf("====================================\n");

    for (int i = 0; i < k; i++)
    {
        if (simulation2Battlefield.battleship.destroyed == 1)
        {
            printf("\nBattleship has been destroyed.");
            printf("\nSimulation stopped at Point %d.\n", i + 1);
            break;
        }

        runSimulationAtPoint(
            &simulation2Battlefield,
            points[i].x,
            points[i].y,
            i + 1,
            (i + 1 >= jamPoint) ? jamAngle : 0
        );

        printf("\nEscort Ships destroyed so far: %d\n",
            countDestroyedEscorts(
                simulation2Battlefield.escorts,
                simulation2Battlefield.numberOfEscorts
            ));
    }

    saveFinalBattlefield(&battlefield);

    saveSimulation2FinalBattlefield(
        &simulation2Battlefield
    );

}

void battleshipProperties()
{
    printf("\n========================================\n");
    printf("        BATTLESHIP PROPERTIES\n");
    printf("========================================\n");

    printf("Battleship Types: U, M, R, S\n");
    printf("Maximum Shell Velocity: Configured during simulation\n");
    printf("Firing Interval: Configured during simulation\n");
    printf("Minimum Firing Angle: 0 - 90 degrees\n");
    printf("Maximum Firing Angle: 90 degrees\n");

    printf("\nPress Enter to return...");
    getchar();
    getchar();
}

void escortShipSettings()
{
    printf("\n========================================\n");
    printf("         ESCORT SHIP SETTINGS\n");
    printf("========================================\n");

    printf("Escort Ship Types: EA, EB, EC, ED, EE\n");

    printf("\nEA:\n");
    printf("  Velocity Range: 0 - 1.2 x Battleship Vmax\n");
    printf("  Angle Range: 20 - 90 degrees\n");
    printf("  Impact Power: 0.08\n");
    printf("  Firing Interval: 4.00 seconds\n");

    printf("\nEB:\n");
    printf("  Velocity Range: 0 - 0.9 x Battleship Vmax\n");
    printf("  Angle Range: 30 - 90 degrees\n");
    printf("  Impact Power: 0.06\n");
    printf("  Firing Interval: 5.00 seconds\n");

    printf("\nEC:\n");
    printf("  Velocity Range: 0 - 0.8 x Battleship Vmax\n");
    printf("  Angle Range: 25 - 90 degrees\n");
    printf("  Impact Power: 0.07\n");
    printf("  Firing Interval: 3.00 seconds\n");

    printf("\nED:\n");
    printf("  Velocity Range: 0 - 0.7 x Battleship Vmax\n");
    printf("  Angle Range: 50 - 90 degrees\n");
    printf("  Impact Power: 0.05\n");
    printf("  Firing Interval: 6.00 seconds\n");

    printf("\nEE:\n");
    printf("  Velocity Range: 0 - 0.6 x Battleship Vmax\n");
    printf("  Angle Range: 70 - 90 degrees\n");
    printf("  Impact Power: 0.04\n");
    printf("  Firing Interval: 7.00 seconds\n");

    printf("\nPress Enter to return...");
    getchar();
    getchar();
}

void seedSettings()
{
    unsigned int seed;

    printf("\n========================================\n");
    printf("              SEED VALUE\n");
    printf("========================================\n");

    printf("Enter seed value: ");
    scanf("%u", &seed);

    srand(seed);

    printf("\nRandom seed has been set to %u.\n", seed);

    printf("\nPress Enter to return...");
    getchar();
    getchar();
}

void instructionsMenu()
{
    printf("\n========================================\n");
    printf("           SIMULATOR INSTRUCTIONS\n");
    printf("========================================\n");

    printf("\nOBJECTIVE\n");
    printf("----------------------------------------\n");
    printf("The Battleship tries to destroy as many\n");
    printf("Escort Ships as possible while reducing\n");
    printf("the damage received from them.\n");

    printf("\nHOW THE SIMULATION WORKS\n");
    printf("----------------------------------------\n");
    printf("1. Set up the Battleship properties.\n");
    printf("2. Set up the Escort Ship properties.\n");
    printf("3. Set the random seed if required.\n");
    printf("4. Choose the number of movement points.\n");
    printf("5. The Battleship attacks Escort Ships\n");
    printf("   that are within its attacking range.\n");
    printf("6. Escort Ships can return fire.\n");
    printf("7. The simulation continues through the\n");
    printf("   movement points until completed or the\n");
    printf("   Battleship is destroyed.\n");

    printf("\nMENU CONTROLS\n");
    printf("----------------------------------------\n");
    printf("Enter the number corresponding to a menu\n");
    printf("option to select it.\n");

    printf("\nFILES\n");
    printf("----------------------------------------\n");
    printf("Simulation results and attack information\n");
    printf("are saved to text files for later review.\n");

    printf("\n========================================\n");

    printf("\nPress Enter to return...");
    getchar();
    getchar();
}

void setupMenu();
void battleshipProperties();
void escortShipSettings();
void seedSettings();
void instructionsMenu();
void simulationStatistics();

void setupMenu()
{
    int choice;

    while (1)
    {
        printf("\n========================================\n");
        printf("              SETUP MENU\n");
        printf("========================================\n");
        printf("1. Battleship Properties\n");
        printf("2. Escort Ship Settings\n");
        printf("3. Seed Value\n");
        printf("4. Return to Main Menu\n");
        printf("========================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                battleshipProperties();
                break;

            case 2:
                escortShipSettings();
                break;

            case 3:
                seedSettings();
                break;

            case 4:
                return;

            default:
                printf("\nInvalid choice. Please enter 1-4.\n");
        }
    }
}

void simulationStatistics()
{
    FILE *file;
    char line[256];
    int choice;
    
    while (1)
    {
        printf("\n========================================\n");
        printf("        SIMULATION STATISTICS\n");
        printf("========================================\n");
        printf("1. Initial Conditions\n");
        printf("2. Final Battlefield\n");
        printf("3. Simulation 2 Final Battlefield\n");
        printf("4. Attack Order\n");
        printf("5. Return to Main Menu\n");
        printf("========================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 5)
        {
            return;
        }

        if (choice == 1)
        {
            file = fopen("initial_conditions.txt", "r");
        }
        else if (choice == 2)
        {
            file = fopen("final_battlefield.txt", "r");
        }
        else if (choice == 3)
        {
            file = fopen("simulation2_final_battlefield.txt", "r");
        }
        else if (choice == 4)
        {
            file = fopen("attack_order.txt", "r");
        }
        else
        {
            printf("\nInvalid choice. Please enter 1-5.\n");
            continue;
        }

        if (file == NULL)
        {
            printf("\nCould not open the selected file.\n");
        }
        else
        {
            printf("\n========================================\n");

            while (fgets(line, sizeof(line), file) != NULL)
            {
                printf("%s", line);
            }

            printf("\n========================================\n");

            fclose(file);
        }

        printf("\nPress Enter to return to Statistics Menu...");
        getchar();
        getchar();
    }
}

int main()
{
    int choice;

    srand(time(NULL));

    while (1)
    {
        printf("\n========================================\n");
        printf("     ADVANCED NAVAL BATTLE SIMULATOR\n");
        printf("========================================\n");
        printf("1. Start Simulation\n");
        printf("2. Setup\n");
        printf("3. View Instructions\n");
        printf("4. Simulation Statistics\n");
        printf("5. Exit\n");
        printf("========================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                startSimulation();
                break;

            case 2:
                setupMenu();
                break;

            case 3:
                instructionsMenu();
                break;

            case 4:
                simulationStatistics();
                break;

            case 5:
            {
                char confirm;

                printf("\nAre you sure you want to exit? (Y/N): ");
                scanf(" %c", &confirm);

                if (confirm == 'Y' || confirm == 'y')
                {
                    printf("\nThank you for using the Advanced Naval Battle Simulator.\n");
                    return 0;
                }
                else if (confirm == 'N' || confirm == 'n')
                {
                    printf("\nReturning to Main Menu...\n");
                }
                else
                {
                    printf("\nInvalid input. Returning to Main Menu...\n");
                }

                break;
            }

            default:
                printf("\nInvalid choice. Please enter 1-5.\n");
        }
    }
}
