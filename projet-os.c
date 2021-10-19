#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "structure.h"

void generateChilds(int *pId, int *childId, int numberOfCars)
{
    for (int i = 0; i < numberOfCars; i++)
    {
        *pId = fork();

        if (!*pId)
        {
            *childId = i;
            break;
        }
    }
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

void main(void)
{
    int shmId;
    int pId;
    int childId;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    int numberOfCars = sizeof(carsNumber) / 4;

    shareMemory *shareMemory;

    shmId = shmget(1027, sizeof(*shareMemory), IPC_CREAT | 0666);
    shareMemory = shmat(shmId, NULL, 0);

    //Stockage du seed dans la memoire partagee pour permettre aux fils de la modifier.
    shareMemory->seed = time(NULL);

    generateChilds(&pId, &childId, numberOfCars);

    if (!pId)
    {
        generateTimesP1(shareMemory, childId);
    }
    else
    {
        for (int i = 0; i < numberOfCars; i++)
        {
            printf("%.3f\n", shareMemory->car[i].sector[0]);
            printf("%.3f\n", shareMemory->car[i].sector[1]);
            printf("%.3f\n", shareMemory->car[i].sector[2]);
            printf("\n\n\n");
        }
    }
}
