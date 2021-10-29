#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "colors.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key);

void choseRace(int *chosenRace, float *timeOfRace);

void initializeGPRelativeData(sharedMemory *sharedMemory, int *carNumbers);

void initializeRaceRelativeData(sharedMemory *sharedMemory);

void generateChilds(int *pId, int *childId);

void initializeBestSectors(sharedMemory *sharedMemory);

void findBestSectors(sharedMemory *sharedMemory);

void sortCarsByBestLap(sharedMemory *sharedMemory);

int display(sharedMemory *sharedMemory);

void eliminate5LastCars(sharedMemory *sharedMemory);