#include "mainProcess.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key)
{
    shmId = shmget(key, sizeof(*sharedMemory), IPC_CREAT | 0666);
    return shmat(shmId, NULL, 0);
}

void choseRace(int *chosenRace, int *timeOfRace)
{
    printf("Choix de la course :\n");
    printf("P1 : 1\n");
    printf("P2 : 2\n");
    printf("P3 : 3\n");
    printf("\n");
    printf("Q1 : 4\n");
    printf("Q2 : 5\n");
    printf("Q3 : 6\n");
    printf("\n");
    printf("Quitter le programme : 0\n");
    scanf("%d", chosenRace);

    if (!*chosenRace)
    {
        exit(0);
    }
    else if (*chosenRace == 1 || *chosenRace == 2)
    {
        *timeOfRace = 5400;
    }
    else if (*chosenRace == 3)
    {
        *timeOfRace = 3600;
    }
    else if (*chosenRace == 4)
    {
        *timeOfRace = 1080;
    }
    else if (*chosenRace == 5)
    {
        *timeOfRace = 900;
    }
    else
    {
        *timeOfRace = 720;
    }
}

void generateChilds(int *pId, int *childId)
{
    for (int i = 0; i < 20; i++)
    {
        *pId = fork();

        if (!*pId)
        {
            *childId = i;
            break;
        }
    }
}

void initializeBestSectors(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->bestSectorTimes[i] = INFINITY;
    }
}

void initializeIsOut(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->car[i].isEliminated = 0;
    }
}

void findBestSectors(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (sharedMemory->car[j].sector[i] < sharedMemory->bestSectorTimes[i])
            {
                sharedMemory->bestSectorTimes[i] = sharedMemory->car[j].sector[i];
                sharedMemory->bestSectorCars[i] = sharedMemory->car[j].carNumber;
            }
        }
    }
}

void sortCarsByBestLap(sharedMemory *sharedMemory)
{
    int i, j;
    struct car temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sharedMemory->car[j].bestLap > sharedMemory->car[j + 1].bestLap)
            {
                temp = sharedMemory->car[j + 1];
                sharedMemory->car[j + 1] = sharedMemory->car[j];
                sharedMemory->car[j] = temp;
            }
        }
    }
}

int display(sharedMemory *sharedMemory)
{
    if (sharedMemory->numberOfCarsFinished == 20)
    {
        printf("\n|-------------------------------------|\n");
        printf("| Eliminated cars : ");

        for (int i = 0; i < 20; i++)
        {
            if (sharedMemory->car[i].isEliminated)
            {
                printf("%2d ", sharedMemory->car[i].carNumber);
            }
        }
        printf("|");
        printf("\n|-------------------------------------|\n");

        return 0;
    }
    system("clear");
    printf("|------------------------------------------------------------------------------------------------------|\n");
    printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | best lap time  | total time  | pit stop(s) | out |\n");
    for (int i = 0; i < 20; i++)
    {
        if (sharedMemory->car[i].isOut)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(RED "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else if (sharedMemory->car[i].isPitStop)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(YEL "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else if (i == 0)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(GRN "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n",
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
    }
    printf("|------------------------------------------------------------------------------------------------------|\n\n");
    printf("|--------------------------------------------------------|\n");
    printf("| best sector 1    | best sector 2    | best sector 3    |\n");
    printf("|--------------------------------------------------------|\n");
    for (int i = 0; i < 3; i++)
    {
        printf("|  %2d : %.3f     ",
               sharedMemory->bestSectorCars[i],
               sharedMemory->bestSectorTimes[i]);
    }
    printf("|\n|--------------------------------------------------------|\n");
}

void storeCarsInfoForSorting(sharedMemory *sharedMemory, sortedCars *sortedCars)
{
    for (int i = 0; i < 20; i++)
    {
        sortedCars[i].carNumber = sharedMemory->car[i].carNumber;
        sortedCars[i].numberOfLaps = sharedMemory->car[i].numberOfLaps;
        sortedCars[i].totalTime = sharedMemory->car[i].totalTime;
    }
}

void calculateAverageSpeed(sortedCars *sortedCars)
{
    for (int i = 0; i < 20; i++)
    {
        sortedCars[i].averageSpeed = sortedCars[i].totalTime / sortedCars[i].numberOfLaps;
    }
}

void sortCarsByAverageSpeed(sortedCars *sortedCars)
{
    int i, j;
    struct sortedCars temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sortedCars[j].averageSpeed > sortedCars[j + 1].averageSpeed)
            {
                temp = sortedCars[j + 1];
                sortedCars[j + 1] = sortedCars[j];
                sortedCars[j] = temp;
            }
        }
    }
}

void eliminate5LastCars(sharedMemory *sharedMemory, sortedCars *sortedCars)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (sharedMemory->car[i].carNumber == sortedCars[19 - j].carNumber)
            {
                sharedMemory->car[i].isEliminated = 1;
            }
        }
    }
}
