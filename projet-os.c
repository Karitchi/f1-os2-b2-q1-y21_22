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

void attributeNumberToEachCar(shareMemory *shareMemory, int carsNumber[], int childId)
{
    shareMemory->car[childId].carNumber = carsNumber[childId];
}

float generateRandomNumber(shareMemory *shareMemory)
{
    srand(shareMemory->seed);
    shareMemory->seed++;
    return (float)rand() / (float)(RAND_MAX)*9 + 40;
}

void generateTimesP1(shareMemory *shareMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        shareMemory->car[childId].sector[i] = generateRandomNumber(shareMemory);
    }
}

void calculateLapTime(shareMemory *shareMemory, int childId)
{
    shareMemory->car[childId].lapTime = 0;

    for (int i = 0; i < 3; i++)
    {
        shareMemory->car[childId].lapTime += shareMemory->car[childId].sector[i];
    }
}

void calculateTimeSpent(shareMemory *shareMemory, int childId)
{
    shareMemory->car[childId].timeSpent += shareMemory->car[childId].lapTime;
}

void main(void)
{
    int shmId;
    int pId;
    int childId;
    int numberOfCarsFinished = 0;
    float timeOfP1 = 5400;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

    shareMemory *shareMemory;

    shmId = shmget(1011, sizeof(*shareMemory), IPC_CREAT | 0666);
    shareMemory = shmat(shmId, NULL, 0);

    //Stockage du seed dans la memoire partagee pour permettre aux fils de la modifier.
    shareMemory->seed = time(NULL);

    generateChilds(&pId, &childId);

    if (!pId)
    {
        shareMemory->car[childId].timeSpent = 0;

        attributeNumberToEachCar(shareMemory, carsNumber, childId);
        int i = 0;
        while (shareMemory->car[childId].timeSpent < timeOfP1)
        {
            generateTimesP1(shareMemory, childId);
            calculateLapTime(shareMemory, childId);
            calculateTimeSpent(shareMemory, childId);
            sleep(0.5);
        }
        numberOfCarsFinished++;
    }
    else
    {
        while (numberOfCarsFinished != 20)
        {
            for (int i = 0; i < 20; i++)
            {
                printf("Car : %d\n", shareMemory->car[i].carNumber);
                printf("Lap time : %.3f\n", shareMemory->car[i].lapTime);
                printf("time spent : %.3f\n", shareMemory->car[i].timeSpent);
                for (int j = 0; j < 3; j++)
                {
                    printf("%.3f\n", shareMemory->car[i].sector[j]);
                }
                printf("\n");
            }
            sleep(1);
        }
    }
}
