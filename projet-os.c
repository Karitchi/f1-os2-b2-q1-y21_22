#include "include.h"

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
        sharedMemory->car[childId].pitStops = 0;

        attributeNumberToEachCar(sharedMemory, carsNumber, childId);

        while (sharedMemory->car[childId].totalTime < timeOfP1P2)
        {

            generateTimesP1(sharedMemory, childId);
            calculateLapTime(sharedMemory, childId);
            calculateTotalTime(sharedMemory, childId);
            generatePitStops(sharedMemory, childId);
            sleep(1);
        }
        sharedMemory->numberOfCarsFinished++;
    }
    else
    {
        while (sharedMemory->numberOfCarsFinished != 20)
        {
            BubbleSort(sharedMemory);
            display(sharedMemory);
            sleep(1);
        }
    }
}
