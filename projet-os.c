#include "include.h"

void main(void)
{
    int shmId, pId, childId;
    char userRaceChoice[2];
    const int timeOfP1P2 = 500;
    const int timeOfP3 = 3600;
    const int RACE_LENGTH = 305;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    struct sortedCars sortedCars[20];
    sharedMemory *sharedMemory;

    // scanf("%s\n", userRaceChoice);

    // Create shared memory and store it in struct.
    shmId = shmget(6, sizeof(*sharedMemory), IPC_CREAT | 0666);
    sharedMemory = shmat(shmId, NULL, 0);

    //Store time in seed to allow random times to be generated.
    sharedMemory->seed = time(NULL);

    sharedMemory->numberOfCarsFinished = 0;

    generateChilds(&pId, &childId);

    if (!pId)
    {
        sharedMemory->car[childId].totalTime = 0;
        sharedMemory->car[childId].isOut = 0;
        sharedMemory->car[childId].numberOfLaps = 0;

        //Attribute a number to each car.
        sharedMemory->car[childId].carNumber = carsNumber[childId];

        // Attribute an infinite number as first best lap.
        sharedMemory->car[childId].bestLap = INFINITY;

        while (sharedMemory->car[childId].totalTime < timeOfP1P2 && !sharedMemory->car[childId].isOut)
        {
            sharedMemory->car[childId].isPitStop = 0;
            generateSectorsTimesP1(sharedMemory, childId);
            calculateLapTime(sharedMemory, childId);
            findBestLap(sharedMemory, childId);
            calculateTotalTime(sharedMemory, childId);
            generatePitStops(sharedMemory, childId);
            generateOut(sharedMemory, childId, pId);
            sharedMemory->car[childId].numberOfLaps++;
            sleep(1);
        }
        sharedMemory->numberOfCarsFinished++;
    }
    else
    {
        initializeBestSectors(sharedMemory);
        initializeIsOut(sharedMemory);
        while (sharedMemory->numberOfCarsFinished != 20)
        {
            findBestSectors(sharedMemory, childId);
            sortCarsByBestLap(sharedMemory);
            display(sharedMemory);
            sleep(1);
        }
        if (userRaceChoice[0] == "q")
        {
            storeCarsInfoForSorting(sharedMemory, sortedCars);
            calculateAverageSpeed(sortedCars);
            sortCarsByAverageSpeed(sortedCars);
            eliminate5LastCars(sharedMemory, sortedCars);
        }
    }
}
