#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "struct.h"
#include "mainProcess.c"
#include "childProcess.c"

// todo fichier recap
// todo nombre de seconde d'avance

void main(void)
{
    int shmId, pId, childId, chosenRace, timeOfRace;
    int key = 777;
    const int RACE_LENGTH = 305;
    int carsNumber[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    struct sortedCars sortedCars[20];
    sharedMemory *sharedMemory = createSharedMemory(sharedMemory, shmId, key);

    sharedMemory->seed = time(NULL);

    choseRace(&chosenRace, &timeOfRace);
    generateChilds(&pId, &childId);

    if (!pId)
    {
        initializeCountersTo0(sharedMemory, childId);

        // Attribute an infinite number as first best lap.
        sharedMemory->car[childId].bestLap = INFINITY;

        //Attribute a number to each car.
        sharedMemory->car[childId].carNumber = carsNumber[childId];

        // while time is not up, car is not out, car is not eliminated
        while (
            sharedMemory->car[childId].totalTime < timeOfRace &&
            !sharedMemory->car[childId].isOut &&
            !sharedMemory->car[childId].isEliminated)
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
        if (chosenRace == 4 || chosenRace == 5)
        {
            storeCarsInfoForSorting(sharedMemory, sortedCars);
            calculateAverageSpeed(sortedCars);
            sortCarsByAverageSpeed(sortedCars);
            eliminate5LastCars(sharedMemory, sortedCars);
            display(sharedMemory);
        }
    }
}
