#include <stdio.h>

#include "simulation.h"
#include "battlefield.h"
#include "battleship.h"
#include "escort.h"
#include "physics.h"

void runBattleSimulation(struct Battlefield *battlefield)
{
    int destroyedCount;
    double battleEndTime = 0;

    printf("\n====================================\n");
    printf("        BATTLE SIMULATION\n");
    printf("====================================\n");

    printf("\n--- Battleship Attack ---\n");

    attackAllEscorts(
        &battlefield->battleship,
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    destroyedCount = countDestroyedEscorts(
        battlefield->escorts,
        battlefield->numberOfEscorts
    );

    printf("\nEscort Ships destroyed: %d\n",
           destroyedCount);

    /*
       Find the latest time among all
       successful Battleship attacks.
    */
    for (int i = 0; i < battlefield->numberOfEscorts; i++)
    {
        if (battlefield->escorts[i].destroyed == 1)
        {
            if (battlefield->escorts[i].hitTime > battleEndTime)
            {
                battleEndTime =
                    battlefield->escorts[i].hitTime;
            }
        }
    }

    if (battlefield->battleship.destroyed == 1)
    {
        printf("\nBattleship was destroyed.");
        printf("\nBattle End Time: %.2f seconds\n",
               battleEndTime);
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

    printf("Battle End Time: %.2f seconds\n",
           battleEndTime);
}
