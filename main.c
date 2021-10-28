/*
?semaphores?
!bug: La derniere voiture du premier classement n'est pas bien triee
!bug: Certaines voitures s'arrete avant la fin du temps (parfois)
!bug: Certaines voitures s'affiche plusieurs fois (parfois)


todo: fichier recap
todo: lire la memoire partagee grace a read
todo: semaphores
*/

#include <time.h>
#include <math.h>
#include <unistd.h>
#include <semaphore.h>
#include "struct.h"
#include "mainProcess.c"
#include "childProcess.c"

void main(void)
{
    int shmId, pId, childId, timeOfRace, chosenRace;
    int carsNumbers[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    const int KEY = 499;
    const int RACE_LENGTH = 305;
    sharedMemory *sharedMemory;
    sharedMemory = createSharedMemory(sharedMemory, shmId, KEY);

    sharedMemory->seed = time(NULL);

    initializeGPRelativeData(sharedMemory, carsNumbers);

    while (1)
    {
        choseRace(&chosenRace, &timeOfRace);
        initializeRaceRelativeData(sharedMemory);
        initializeBestSectors(sharedMemory);
        if (chosenRace == 7)
        {
            initializeFinalRace(sharedMemory);
            display(sharedMemory);
            sleep(1);
            addTimesToCars(sharedMemory);
        }
        generateChilds(&pId, &childId);

        if (!pId)
        {
            // while time is not up, car is not out, car is not eliminated
            while (
                sharedMemory->cars[childId].totalTime < timeOfRace &&
                !sharedMemory->cars[childId].isOut &&
                !sharedMemory->cars[childId].isEliminated)
            {
                initializeLapRelativeData(sharedMemory, childId);
                generateSectorsTimes(sharedMemory, childId);
                calculateLapTime(sharedMemory, childId);
                calculateTotalTime(sharedMemory, childId);
                generatePitStops(sharedMemory, childId);
                generateOut(sharedMemory, childId, pId);
                findBestLap(sharedMemory, childId);
                sharedMemory->cars[childId].numberOfLaps++;
                sleep(1);
            }

            sharedMemory->numberOfCarsFinished++;
            exit(0);
        }
        else
        {
            while (sharedMemory->numberOfCarsFinished != 20)
            {
                findBestSectors(sharedMemory);
                sortCarsByBestLap(sharedMemory);
                calculateInterval(sharedMemory);
                display(sharedMemory);
                sleep(1);
            }
            if (chosenRace == 4 || chosenRace == 5)
            {
                eliminate5LastCars(sharedMemory);
                display(sharedMemory);
            }
            writeDisplayToFile(sharedMemory, chosenRace);
        }
    }
}