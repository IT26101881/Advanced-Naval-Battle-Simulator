#include <stdio.h>

#include "file_handler.h"
#include "battlefield.h"
#include "battleship.h"
#include "escort.h"

void saveInitialConditions(struct Battlefield *battlefield)
{
    FILE *file;

    file = fopen("initial_conditions.txt", "w");

    if (file == NULL)
    {
        printf("Error: Could not create initial_conditions.txt\n");
        return;
    }

    fprintf(file, "===== INITIAL BATTLEFIELD CONDITIONS =====\n\n");

    fprintf(file, "Battlefield Size: %.2f\n\n",
            battlefield->size);

    fprintf(file, "===== BATTLESHIP =====\n");
    fprintf(file, "Type: %c\n",
            battlefield->battleship.type);

    fprintf(file, "Position: (%.2f, %.2f)\n",
            battlefield->battleship.x,
            battlefield->battleship.y);

    fprintf(file, "Maximum Shell Velocity: %.2f\n\n",
            battlefield->battleship.maxVelocity);

    fprintf(file, "Number of Escort Ships: %d\n\n",
            battlefield->numberOfEscorts);

    fprintf(file, "===== ESCORT SHIPS =====\n");

    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        fprintf(file, "\nEscort Ship %d\n",
                battlefield->escorts[i].id);

        fprintf(file, "Type: %s\n",
                battlefield->escorts[i].type);

        fprintf(file, "Position: (%.2f, %.2f)\n",
                battlefield->escorts[i].x,
                battlefield->escorts[i].y);

        fprintf(file, "Minimum Velocity: %.2f\n",
                battlefield->escorts[i].minVelocity);

        fprintf(file, "Maximum Velocity: %.2f\n",
                battlefield->escorts[i].maxVelocity);

        fprintf(file, "Actual Velocity: %.2f\n",
                battlefield->escorts[i].velocity);

        fprintf(file, "Minimum Angle: %.2f degrees\n",
                battlefield->escorts[i].minAngle);

        fprintf(file, "Maximum Angle: %.2f degrees\n",
                battlefield->escorts[i].maxAngle);

        fprintf(file, "Actual Angle: %.2f degrees\n",
                battlefield->escorts[i].angle);

        fprintf(file, "Impact Power: %.2f\n",
                battlefield->escorts[i].impactPower);
    }

    fclose(file);

    printf("\nInitial conditions saved to initial_conditions.txt\n");
}

void saveFinalBattlefield(struct Battlefield *battlefield)
{
    FILE *file;

    file = fopen("final_battlefield.txt", "w");

    if (file == NULL)
    {
        printf("Error: Could not create final_battlefield.txt\n");
        return;
    }

    fprintf(file, "===== FINAL BATTLEFIELD =====\n\n");

    fprintf(file, "===== BATTLESHIP =====\n");

    fprintf(file, "Type: %c\n",
            battlefield->battleship.type);

    fprintf(file, "Position: (%.2f, %.2f)\n",
            battlefield->battleship.x,
            battlefield->battleship.y);

    if (battlefield->battleship.destroyed == 1)
    {
        fprintf(file, "Status: Destroyed\n");
    }
    else
    {
        fprintf(file, "Status: Survived\n");
    }

    fprintf(file, "\n===== ESCORT SHIPS =====\n");

    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        fprintf(file, "\nEscort Ship %d\n",
                battlefield->escorts[i].id);

        fprintf(file, "Type: %s\n",
                battlefield->escorts[i].type);

        fprintf(file, "Position: (%.2f, %.2f)\n",
                battlefield->escorts[i].x,
                battlefield->escorts[i].y);

        if (battlefield->escorts[i].destroyed == 1)
        {
            fprintf(file, "Status: Destroyed\n");

            fprintf(file, "Time to Hit: %.2f seconds\n",
                    battlefield->escorts[i].hitTime);
        }
        else
        {
            fprintf(file, "Status: Survived\n");
        }
    }

    fclose(file);

    printf("Final battlefield saved to final_battlefield.txt\n");
}
