/*
?semaphores?
!bug: La derniere voiture du premier classement n'est pas bien triee
!bug: Certaines voitures s'arrete avant la fin du temps (parfois)
!bug: Certaines voitures s'affiche plusieurs fois (parfois)


todo: nettoyage du code
todo: semaphores
todo: appuyer sur enter pour continuer
todo: lire et ecrire dans un fichier les resultats

todo: -voitures terminent tour et incrementent numberOfCarFinishedLap (1 ere semaphore)
todo: -quand numberOfCarFinishedLap = 20 le deuxieme semaphore est incremente par la derniere voiture
todo: -le pere peut alors lire et decremente le semaphore 
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
    int shmId, pId, childId, chosenRace, trackLength;
    float timeOfRace;
    int numberOfLapsLeft = 1000;
    int carsNumbers[] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    const int KEY = 2345;
    const int RACE_LENGTH = 305;
    sharedMemory *sharedMemory;
    sharedMemory = createSharedMemory(sharedMemory, shmId, KEY);
    int value = 0;

    sharedMemory->nbWriter = 20;

    sharedMemory->seed = time(NULL);

    sem_init(&sharedMemory->readerSemaphore, 1, 0);
    sem_init(&sharedMemory->writerSemaphore, 1, 20);

    initializeGPRelativeData(sharedMemory, carsNumbers);

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
            while (
                sharedMemory->cars[childId].totalTime < timeOfRace &&
                !sharedMemory->cars[childId].isOut &&
                !sharedMemory->cars[childId].isEliminated &&
                numberOfLapsLeft)
            {
                sem_wait(&sharedMemory->writerSemaphore);
                sharedMemory->nbWriter--;
                initializeLapRelativeData(sharedMemory, childId);
                generateSectorsTimes(sharedMemory, childId);
                calculateLapTime(sharedMemory, childId);
                calculateTotalTime(sharedMemory, childId);
                generatePitStops(sharedMemory, childId);
                generateOut(sharedMemory, childId, pId);
                findBestLap(sharedMemory, childId);
                sharedMemory->cars[childId].numberOfLaps++;
                numberOfLapsLeft--;
                sharedMemory->nbWriter++;

                if (sharedMemory->nbWriter == 20)
                {
                    sem_post(&sharedMemory->readerSemaphore);
                }
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
                for (int i = 0; i < 20; i++)
                {
                    sem_post(&sharedMemory->writerSemaphore);
                }

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
    sem_destroy(&sharedMemory->readerSemaphore);
    sem_destroy(&sharedMemory->writerSemaphore);
}