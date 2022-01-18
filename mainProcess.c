#include "mainProcess.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key)
{
    shmId = shmget(key, sizeof(*sharedMemory), IPC_CREAT | 0666);
    return shmat(shmId, NULL, 0);
}

void initializeData(sharedMemory *sharedMemory, int *carNumbers)
{
    sem_init(&sharedMemory->readerSemaphore, 1, 0);
    sem_init(&sharedMemory->writerSemaphore, 1, 20);

    sharedMemory->nbWriter = 20;
    sharedMemory->seed = time(NULL);

    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i].isEliminated = 0;
        sharedMemory->cars[i].carNumber = carNumbers[i];
    }
}

void initializeFinalRace(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i].isEliminated = 0;
    }
}

void choseRace(int *chosenRace, float *timeOfRace)
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
    case 6:
        *timeOfRace = 720;
        break;
    case 7:
        *timeOfRace = INFINITY;
    }
}

void calculateNumberOfLaps(int *trackLength, int *numberOfLaps, const int RACE_LENGTH)
{
    printf("How long is the track? (km) ");
    scanf("%d", trackLength);
    *numberOfLaps = RACE_LENGTH / *trackLength;
}

void initializeRaceRelativeData(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sharedMemory->cars[i].sector[j] = INFINITY;
        }
        sharedMemory->numberOfCarsFinished = 0;
        sharedMemory->cars[i].lapTime = 0;
        sharedMemory->cars[i].totalTime = 0;
        sharedMemory->cars[i].isOut = 0;
        sharedMemory->cars[i].isPitStop = 0;
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

void addTimesToCars(sharedMemory *sharedMemory)
{
    float timeAdded = 0;

    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i].totalTime += timeAdded;
        timeAdded += 0.5;
    }
}

void findBestSectors(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (sharedMemory->cars[j].sector[i] < sharedMemory->bestSectorTimes[i] &&
                !sharedMemory->cars[j].isEliminated)
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

void placeOutCarsLastPlace(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        if (sharedMemory->cars[i].isOut)
        {
            sharedMemory->cars[i].totalTime = INFINITY;
        }
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
            if (sharedMemory->cars[j].totalTime > sharedMemory->cars[j + 1].totalTime)
            {
                temp = sharedMemory->cars[j + 1];
                sharedMemory->cars[j + 1] = sharedMemory->cars[j];
                sharedMemory->cars[j] = temp;
            }
        }
    }
}

void calculateIntervalBestLap(sharedMemory *sharedMemory)
{
    sharedMemory->cars[0].interval = 0;
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i + 1].interval = sharedMemory->cars[i + 1].bestLap - sharedMemory->cars[i].bestLap;
    }
}

void calculateIntervalAvgSpeed(sharedMemory *sharedMemory)
{
    sharedMemory->cars[0].interval = 0;
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->cars[i + 1].interval = sharedMemory->cars[i + 1].totalTime - sharedMemory->cars[i].totalTime;
    }
}

int display(sharedMemory *sharedMemory)
{
    // system("clear");
    printf("|--------------------------------------------------------------------------------------------------------------------|\n");
    printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | best lap time  | total time  | pit stop(s) | out |  interval   |\n");
    for (int i = 0; i < 20; i++)
    {
        if (sharedMemory->cars[i].isEliminated)
        {
            // printf("|--------------------------------------------------------------------------------------------------------------------|\n");

            // printf(RED "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n" RESET,
            //        sharedMemory->cars[i].carNumber,
            //        sharedMemory->cars[i].sector[0],
            //        sharedMemory->cars[i].sector[1],
            //        sharedMemory->cars[i].sector[2],
            //        sharedMemory->cars[i].lapTime,
            //        sharedMemory->cars[i].bestLap,
            //        sharedMemory->cars[i].totalTime,
            //        sharedMemory->cars[i].isPitStop,
            //        sharedMemory->cars[i].isOut,
            //        sharedMemory->cars[i].interval);
        }
        else if (sharedMemory->cars[i].isOut)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf(YEL "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else if (sharedMemory->cars[i].isPitStop)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf(BLU "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else if (i == 0)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf(GRN "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n" RESET,
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
    }
    printf("|--------------------------------------------------------------------------------------------------------------------|\n");

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

void incrementwriterSem(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20 - sharedMemory->numberOfCarsFinished; i++)
    {
        sem_post(&sharedMemory->writerSemaphore);
    }
}

void displayWithoutColors(sharedMemory *sharedMemory)
{
    printf("|--------------------------------------------------------------------------------------------------------------------|\n");
    printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | best lap time  | total time  | pit stop(s) | out |  interval   |\n");
    for (int i = 0; i < 20; i++)
    {
        if (sharedMemory->cars[i].isEliminated)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");

            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else if (sharedMemory->cars[i].isOut)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else if (sharedMemory->cars[i].isPitStop)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else if (i == 0)
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
        else
        {
            printf("|--------------------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |  +%8.3f  |\n",
                   sharedMemory->cars[i].carNumber,
                   sharedMemory->cars[i].sector[0],
                   sharedMemory->cars[i].sector[1],
                   sharedMemory->cars[i].sector[2],
                   sharedMemory->cars[i].lapTime,
                   sharedMemory->cars[i].bestLap,
                   sharedMemory->cars[i].totalTime,
                   sharedMemory->cars[i].isPitStop,
                   sharedMemory->cars[i].isOut,
                   sharedMemory->cars[i].interval);
        }
    }
    printf("|--------------------------------------------------------------------------------------------------------------------|\n");

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

int writeDisplayToFile(sharedMemory *sharedMemory, int chosenRace)
{
    char fileName[7][256] = {"P1.txt", "P2.txt", "P3.txt", "Q1.txt", "Q2.txt", "Q3.txt", "Final.txt"};
    // Save stdout to reset it after.
    int savedStdout = dup(1);

    // Open or create file.
    int fd = open(fileName[chosenRace - 1], O_WRONLY | O_CREAT, 0777);

    // Set stdout to the file.
    dup2(fd, STDOUT_FILENO);

    displayWithoutColors(sharedMemory);

    // Reset stdout.
    dup2(savedStdout, 1);

    close(fd);
    close(savedStdout);
}

void eliminate5LastCars(sharedMemory *sharedMemory)
{
    int numberEliminatedCars = 0;

    for (int i = 19; i > 0; i--)
    {
        if (!sharedMemory->cars[i].isEliminated)
        {
            sharedMemory->cars[i].isEliminated = 1;
            numberEliminatedCars++;
        }
        if (numberEliminatedCars == 5)
        {
            break;
        }
    }
}

void DestroySem(sharedMemory *sharedMemory)
{
    sem_destroy(&sharedMemory->readerSemaphore);
    sem_destroy(&sharedMemory->writerSemaphore);
}