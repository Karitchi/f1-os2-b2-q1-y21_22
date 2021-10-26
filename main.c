/*
?semaphores?
!bug: La derniere voiture du premier classement n'est pas bien triee
!bug: Certaines voitures s'arrete avant la fin du temps (parfois)
!bug: Certaines voitures s'affiche plusieurs fois (parfois)

todo: fichUncommittedier recap
todo: nombre de seconde d'avances

?nombre de secondes d'avance par rapport au tour?
*/

#include <time.h>
#include <math.h>
#include <unistd.h>
#include <semaphore.h>
#include "struct.h"
#include "mainProcess.c"
#include "childProcess.c"

sem_t semaphore;

void main(void)
{
    int shmId, pId, childId, chosenRace, timeOfRace;
    int carsNumbers[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    const int KEY = 777;
    const int RACE_LENGTH = 305;
    sharedMemory *sharedMemory;
    sharedMemory = createSharedMemory(sharedMemory, shmId, KEY);

    sharedMemory->seed = time(NULL);

    initializeGPRelativeData(sharedMemory, carsNumbers);
    choseRace(&chosenRace, &timeOfRace);
    initializeRaceRelativeData(sharedMemory);
    initializeBestSectors(sharedMemory);
    sem_init(&semaphore, 1, 1);
    generateChilds(&pId, &childId);

    if (!pId)
    {

        // while time is not up, car is not out, car is not eliminated
        while (
            sharedMemory->car[childId].totalTime < timeOfRace &&
            !sharedMemory->car[childId].isOut &&
            !sharedMemory->car[childId].isEliminated)
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

        sem_wait(&semaphore);
        sharedMemory->numberOfCarsFinished++;
        sem_post(&semaphore);
    }
    else
    {
        while (sharedMemory->numberOfCarsFinished != 20)
        {
            findBestSectors(sharedMemory);
            sortCarsByBestLap(sharedMemory);
            calculateAvgSpeed(sharedMemory);
            sortCarsByAvgSpeed(sharedMemory);
            calculateTimeDifference(sharedMemory);
            display(sharedMemory);
            sleep(1);
        }
        if (chosenRace == 4 || chosenRace == 5)
        {
            eliminate5LastCars(sharedMemory);
            display(sharedMemory);
        }
    }
}