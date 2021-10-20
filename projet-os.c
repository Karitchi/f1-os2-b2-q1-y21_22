#include "include.h"

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

void storeAllLapTimes(sharedMemory *sharedMemory, int childId)
{
    sharedMemory->allLapTimes[childId] = sharedMemory->car[childId].lapTime;
}

void swap(sharedMemory *sharedMemory, int j)
{
    float temp = sharedMemory->allLapTimes[j];
    sharedMemory->allLapTimes[j] = sharedMemory->allLapTimes[j + 1];
    sharedMemory->allLapTimes[j + 1] = temp;
}

void sortLapTimes(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20 - 1; i++)
    {
        for (int j = 0; j < 20 - i - 1; j++)
        {
            if (sharedMemory->allLapTimes[j] > sharedMemory->allLapTimes[j + 1])
            {
                swap(sharedMemory, j);
            }
        }
    }
    for (int i = 0; i < 20; i++)
    {
        printf("%f\n", sharedMemory->allLapTimes[i]);
    }
    printf("%f\n", sharedMemory->allLapTimes[19]);
}

void main(void)
{
    int shmId;
    int pId;
    int childId;
    int lapLength = 9;
    const int timeOfP1P2 = 1000;
    const int RACE_LENGTH = 305;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

    sharedMemory *sharedMemory;

    shmId = shmget(1662, sizeof(*sharedMemory), IPC_CREAT | 0666);
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
            storeAllLapTimes(sharedMemory, childId);

            sleep(1);
        }
        sharedMemory->numberOfCarsFinished++;
    }
    else
    {
        while (sharedMemory->numberOfCarsFinished != 20)
        {
            sortLapTimes(sharedMemory);

            // for (int i = 0; i < 20; i++)
            // {
            //     printf("%f\n", sharedMemory->allLapTimes[i]);
            // }

            // system("clear");
            // printf("|----------------------------------------------------------------|\n");
            // printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | total time |\n");
            // for (int i = 0; i < 20; i++)
            // {
            //     printf("|----------------------------------------------------------------|\n");
            //     printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |  %8.3f  |\n",
            //            sharedMemory->car[i].carNumber,
            //            sharedMemory->car[i].sector[0],
            //            sharedMemory->car[i].sector[1],
            //            sharedMemory->car[i].sector[2],
            //            sharedMemory->car[i].lapTime,
            //            sharedMemory->car[i].totalTime);
            // }
            // printf("|----------------------------------------------------------------|\n");
            sleep(1);
        }
    }
}
