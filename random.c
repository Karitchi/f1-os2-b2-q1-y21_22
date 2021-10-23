#include "random.h"

float generateRandomNumber(sharedMemory *sharedMemory, int max, int added)
{

    srand(sharedMemory->seed);
    sharedMemory->seed++;
    return (float)rand() / (float)(RAND_MAX)*max + added;
}