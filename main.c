#include <time.h>
#include <math.h>
#include <unistd.h>
#include <semaphore.h>
#include "struct.h"
#include "mainProcess.c"
#include "childProcess.c"

void main(void)
{
    int shmId, pId, childId, chosenRace, trackLength;
    float timeOfRace;
    int numberOfLapsLeft = 1000;
    int carsNumbers[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    const int KEY = 2345;
    const int RACE_LENGTH = 305;
    sharedMemory *sharedMemory;
    sharedMemory = createSharedMemory(sharedMemory, shmId, KEY);
    initializeData(sharedMemory, carsNumbers);

    while (1)
    {
        choseRace(&chosenRace, &timeOfRace);
        initializeRaceRelativeData(sharedMemory);
        initializeBestSectors(sharedMemory);
        if (chosenRace == 7)
        {
            calculateNumberOfLaps(&trackLength, &numberOfLapsLeft, RACE_LENGTH);
            initializeFinalRace(sharedMemory);
            display(sharedMemory);
            sleep(1);
            addTimesToCars(sharedMemory);
        }

        generateChilds(&pId, &childId);

        if (!pId)
        {
            // while time is not up, car is not out, car is not eliminated
            while (sharedMemory->cars[childId].totalTime < timeOfRace &&
                   !sharedMemory->cars[childId].isOut &&
                   !sharedMemory->cars[childId].isEliminated &&
                   numberOfLapsLeft)
            {
                sem_wait(&sharedMemory->writerSemaphore);
                sharedMemory->nbProcessWriting--;
                initializeLapRelativeData(sharedMemory, childId);
                generateSectorsTimes(sharedMemory, childId);
                calculateLapTime(sharedMemory, childId);
                calculateTotalTime(sharedMemory, childId);
                generatePitStops(sharedMemory, childId);
                generateOut(sharedMemory, childId, pId);
                findBestLap(sharedMemory, childId);
                sharedMemory->cars[childId].numberOfLaps++;
                numberOfLapsLeft--;
                sharedMemory->nbProcessWriting++;
                incrementReaderSem(sharedMemory);
                sleep(1);
            }
            sharedMemory->cars[childId].isPitStop = 0;
            sharedMemory->numberOfCarsFinished++;

            exit(0);
        }
        else
        {
            while (sharedMemory->numberOfCarsFinished != 20)
            {
                sem_wait(&sharedMemory->readerSemaphore);
                findBestSectors(sharedMemory);

                if (chosenRace != 7)
                {
                    sortCarsByBestLap(sharedMemory);
                    calculateIntervalBestLap(sharedMemory);
                }
                else
                {
                    placeOutCarsLastPlace(sharedMemory);
                    sortCarsByAvgSpeed(sharedMemory);
                    calculateIntervalAvgSpeed(sharedMemory);
                }

                display(sharedMemory);
                incrementwriterSem(sharedMemory);
                sleep(1);
            }

            if (chosenRace == 4 || chosenRace == 5)
            {
                eliminate5LastCars(sharedMemory);
            }
            display(sharedMemory);
            writeDisplayToFile(sharedMemory, chosenRace);
        }
    }
    DestroySem(sharedMemory);
}