#include "mainProcess.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key)
{
    shmId = shmget(key, sizeof(*sharedMemory), IPC_CREAT | 0666);
    return shmat(shmId, NULL, 0);
}

void initializeGPRelativeData(sharedMemory *sharedMemory, int *carNumbers)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i].isEliminated = 0;
        sharedMemory->cars[i].carNumber = carNumbers[i];
    }
}

void choseRace(int *chosenRace, int *timeOfRace)
{
    printf("Essai 1 : 1\n");
    printf("Essai 2 : 2\n");
    printf("Essai 3 : 3\n");
    printf("\n");
    printf("Qualification 1 : 4\n");
    printf("Qualification 2 : 5\n");
    printf("Qualification 3 : 6\n");
    printf("\n");
    printf("Course : 7\n");
    printf("\n");
    printf("Quitter : 0\n");
    printf("\n");
    printf("Choix : ");

    scanf("%d", chosenRace);

    switch (*chosenRace)
    {
    case 0:
        exit(0);
        break;
    case 1:
    case 2:
        *timeOfRace = 5400;
        break;

    case 3:
        *timeOfRace = 3600;
        break;
    case 4:
        *timeOfRace = 1080;
        break;
    case 5:
        *timeOfRace = 900;
        break;
    default:
        *timeOfRace = 720;
    }
}

void initializeRaceRelativeData(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->numberOfCarsFinished = 0;
        sharedMemory->cars[i].totalTime = 0;
        sharedMemory->cars[i].isOut = 0;
        sharedMemory->cars[i].numberOfLaps = 0;
        sharedMemory->cars[i].bestLap = INFINITY;
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

void findBestSectors(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (sharedMemory->cars[j].sector[i] < sharedMemory->bestSectorTimes[i])
            {
                sharedMemory->bestSectorTimes[i] = sharedMemory->cars[j].sector[i];
                sharedMemory->bestSectorCars[i] = sharedMemory->cars[j].carNumber;
            }
        }
    }
}

void sortCarsByBestLap(sharedMemory *sharedMemory)
{
    int i, j;
    struct cars temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sharedMemory->cars[j].bestLap > sharedMemory->cars[j + 1].bestLap)
            {
                temp = sharedMemory->cars[j + 1];
                sharedMemory->cars[j + 1] = sharedMemory->cars[j];
                sharedMemory->cars[j] = temp;
            }
        }
    }
}

int display(sharedMemory *sharedMemory)
{
    system("clear");
    printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
    printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | best lap time  | total time  | pit stop(s) | out |   time difference   |\n");
    for (int i = 0; i < 20; i++)
    {
        if (sharedMemory->cars[i].isEliminated)
        {
            printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
            printf(RED "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  %2.3f\" ahead of %2d |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].numberOfSecondsAhead,
                   sharedMemory->cars[i + 1].carNumber);
        }
        else if (sharedMemory->cars[i].isOut)
        {
            printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
            printf(YEL "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  %2.3f\" ahead of %2d |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].numberOfSecondsAhead,
                   sharedMemory->cars[i + 1].carNumber);
        }
        else if (sharedMemory->cars[i].isPitStop)
        {
            printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
            printf(BLU "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  %2.3f\" ahead of %2d |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].numberOfSecondsAhead,
                   sharedMemory->cars[i + 1].carNumber);
        }
        else if (i == 0)
        {
            printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
            printf(GRN "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  %2.3f\" ahead of %2d |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].numberOfSecondsAhead,
                   sharedMemory->cars[i + 1].carNumber);
        }
        else
        {
            printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  %2.3f\" ahead of %2d |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].numberOfSecondsAhead,
                   sharedMemory->cars[i + 1].carNumber);
        }
    }
    printf("|----------------------------------------------------------------------------------------------------------------------------|\n");
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

void calculateAvgSpeed(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i].avgSpeed = sharedMemory->cars[i].totalTime / sharedMemory->cars[i].numberOfLaps;
    }
}

void sortCarsByAvgSpeed(sharedMemory *sharedMemory)
{
    int i, j;
    struct cars temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sharedMemory->cars[j].avgSpeed > sharedMemory->cars[j + 1].avgSpeed)
            {
                temp = sharedMemory->cars[j + 1];
                sharedMemory->cars[j + 1] = sharedMemory->cars[j];
                sharedMemory->cars[j] = temp;
            }
        }
    }
}

void calculateTimeDifference(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i].numberOfSecondsAhead = sharedMemory->cars[i + 1].totalTime - sharedMemory->cars[i].totalTime;
    }
}

void eliminate5LastCars(sharedMemory *sharedMemory)
{
    for (int i = 20; i > 14; i--)
    {
        sharedMemory->cars[i].isEliminated = 1;
    }
}
