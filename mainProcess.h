#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "colors.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key);

void initializeGPRelativeData(sharedMemory *sharedMemory, int *carNumbers);

void initializeFinalRace(sharedMemory *sharedMemory);

void choseRace(int *chosenRace, float *timeOfRace);

void calculateNumberOfLaps(int *trackLength, int *numberOfLaps, const int RACE_LENGTH);

void initializeRaceRelativeData(sharedMemory *sharedMemory);

void generateChilds(int *pId, int *childId);

void initializeBestSectors(sharedMemory *sharedMemory);

void addTimesToCars(sharedMemory *sharedMemory);

void findBestSectors(sharedMemory *sharedMemory);

void sortCarsByBestLap(sharedMemory *sharedMemory);

void placeOutCarsLastPlace(sharedMemory *sharedMemory);

void sortCarsByAvgSpeed(sharedMemory *sharedMemory);

void calculateIntervalBestLap(sharedMemory *sharedMemory);

void calculateIntervalAvgSpeed(sharedMemory *sharedMemory);

int display(sharedMemory *sharedMemory);

void displayWithoutColors(sharedMemory *sharedMemory);

int writeDisplayToFile(sharedMemory *sharedMemory, int chosenRace);

void eliminate5LastCars(sharedMemory *sharedMemory);
