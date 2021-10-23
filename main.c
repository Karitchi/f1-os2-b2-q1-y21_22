/*
! Certaines voitures s'affiche plusieurs fois parfois
! La derniere voiture du premier classement n'est pas bien triee

todo fichier recap
todo nombre de seconde d'avance
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
    int shmId, pId, childId, chosenRace, timeOfRace, key;
    const int RACE_LENGTH = 305;
    sharedMemory *sharedMemory;

    int carsNumbers[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    key = 777;
    sharedMemory = createSharedMemory(sharedMemory, shmId, key);
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