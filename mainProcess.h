#include <stdio.h>
#include "colors.h"

void generateChilds(int *pId, int *childId);

void initializeBestSectors(sharedMemory *sharedMemory);

void initializeIsOut(sharedMemory *sharedMemory);

void findBestSectors(sharedMemory *sharedMemory, int childId);

void sortCarsByBestLap(sharedMemory *sharedMemory);

void display(sharedMemory *sharedMemory);

void storeCarsInfoForSorting(sharedMemory *sharedMemory, sortedCars *sortedCars);

void calculateAverageSpeed(sortedCars *sortedCars);

void sortCarsByAverageSpeed(sortedCars *sortedCars);

void eliminate5LastCars(sharedMemory *sharedMemory, sortedCars *sortedCars);
