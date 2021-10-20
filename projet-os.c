#include "include.h"
#include "colors.h"

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

void attributeNumberToEachCar(sharedMemory *sharedMemory, int carsNumber[], int childId)
{
    sharedMemory->car[childId].carNumber = carsNumber[childId];
}

float generateRandomNumber(sharedMemory *sharedMemory)
{
    srand(sharedMemory->seed);
    sharedMemory->seed++;
    return (float)rand() / (float)(RAND_MAX)*9 + 40;
}

void generateTimesP1(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].sector[i] = generateRandomNumber(sharedMemory);
    }
}

void calculateLapTime(sharedMemory *sharedMemory, int childId)
{
    sharedMemory->car[childId].lapTime = 0;

    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].lapTime += sharedMemory->car[childId].sector[i];
    }
}

void calculateTotalTime(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].totalTime += sharedMemory->car[childId].sector[i];
    }
}

void BubbleSort(sharedMemory *sharedMemory)
{
    int i, j;
    struct car temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sharedMemory->car[j].lapTime > sharedMemory->car[j + 1].lapTime)
            {
                temp = sharedMemory->car[j + 1];
                sharedMemory->car[j + 1] = sharedMemory->car[j];
                sharedMemory->car[j] = temp;
            }
        }
    }
}

void main(void)
{
    int shmId, pId, childId;
    int lapLength = 9;
    const int timeOfP1P2 = 1000;
    const int RACE_LENGTH = 305;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

    sharedMemory *sharedMemory;

    shmId = shmget(1663, sizeof(*sharedMemory), IPC_CREAT | 0666);
    sharedMemory = shmat(shmId, NULL, 0);

    //Stockage du seed dans la memoire partagee pour permettre aux fils de la modifier.
    sharedMemory->seed = time(NULL);

    sharedMemory->numberOfCarsFinished = 0;

    generateChilds(&pId, &childId);

    if (!pId)
    {
        sharedMemory->car[childId].totalTime = 0;

        attributeNumberToEachCar(sharedMemory, carsNumber, childId);

        while (sharedMemory->car[childId].totalTime < timeOfP1P2)
        {

            generateTimesP1(sharedMemory, childId);
            calculateLapTime(sharedMemory, childId);
            calculateTotalTime(sharedMemory, childId);
            sleep(1);
        }
        sharedMemory->numberOfCarsFinished++;
    }
    else
    {
        while (sharedMemory->numberOfCarsFinished != 20)
        {
            BubbleSort(sharedMemory);

            system("clear");
            printf("|---------------------------------------------------|\n");
            printf("| car  | sector 1 | sector 2 | sector 3 | lap time  |\n");
            for (int i = 0; i < 20; i++)
            {
                if (i == 0)
                {
                    printf("|---------------------------------------------------|\n");
                    printf(GRN "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |\n" RESET,
                           sharedMemory->car[i].carNumber,
                           sharedMemory->car[i].sector[0],
                           sharedMemory->car[i].sector[1],
                           sharedMemory->car[i].sector[2],
                           sharedMemory->car[i].lapTime);
                }
                else
                {
                    printf("|---------------------------------------------------|\n");
                    printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |\n",
                           sharedMemory->car[i].carNumber,
                           sharedMemory->car[i].sector[0],
                           sharedMemory->car[i].sector[1],
                           sharedMemory->car[i].sector[2],
                           sharedMemory->car[i].lapTime);
                }
            }
            printf("|---------------------------------------------------|\n");
            sleep(1);
        }
    }
}
