#include <stdio.h>
#include "colors.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key);

void choseRace(int *chosenRace, int *timeOfRace);

void generateChilds(int *pId, int *childId);

void initializeBestSectors(sharedMemory *sharedMemory);

void initializeIsOut(sharedMemory *sharedMemory);

void findBestSectors(sharedMemory *sharedMemory, int childId);

void sortCarsByBestLap(sharedMemory *sharedMemory);

int display(sharedMemory *sharedMemory);

void storeCarsInfoForSorting(sharedMemory *sharedMemory, sortedCars *sortedCars);

void calculateAverageSpeed(sortedCars *sortedCars);

void sortCarsByAverageSpeed(sortedCars *sortedCars);

void eliminate5LastCars(sharedMemory *sharedMemory, sortedCars *sortedCars);