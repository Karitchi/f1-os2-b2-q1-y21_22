#include "include.h"

void main(void)
{
    int shmId, pId, childId;
    int lapLength = 9;
    const int timeOfP1P2 = 1000;
    const int RACE_LENGTH = 305;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

    // Create the struct
    sharedMemory *sharedMemory;

    // Create shared memory and store it in struct.
    shmId = shmget(2763, sizeof(*sharedMemory), IPC_CREAT | 0666);
    sharedMemory = shmat(shmId, NULL, 0);

    //Store time in seed to allow random times to be generated.
    sharedMemory->seed = time(NULL);

    sharedMemory->numberOfCarsFinished = 0;

    generateChilds(&pId, &childId);

    if (!pId)
    {
        sharedMemory->car[childId].totalTime = 0;
        sharedMemory->car[childId].pitStops = 0;
        sharedMemory->car[childId].numberOfLaps = 0;

        //Attribute a number to each car.
        sharedMemory->car[childId].carNumber = carsNumber[childId];

        // Attribute an infinite nuber as first best lap.
        sharedMemory->car[childId].bestLap = INFINITY;

        while (sharedMemory->car[childId].totalTime < timeOfP1P2)
        {

            generateSectorsTimesP1(sharedMemory, childId);
            calculateLapTime(sharedMemory, childId);
            findBestLap(sharedMemory, childId);
            calculateTotalTime(sharedMemory, childId);
            generatePitStops(sharedMemory, childId);
            sharedMemory->car[childId].numberOfLaps++;
            sleep(1);
        }
        sharedMemory->numberOfCarsFinished++;
    }
    else
    {
        while (sharedMemory->numberOfCarsFinished != 20)
        {
            findBestSectors(sharedMemory, childId);
            BubbleSort(sharedMemory);
            display(sharedMemory);
            sleep(1);
        }
    }
}
