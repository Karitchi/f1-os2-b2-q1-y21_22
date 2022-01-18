#include "childProcess.h"

void initializeLapRelativeData(sharedMemory *sharedMemory, int childId)
{
    sharedMemory->cars[childId].isPitStop = 0;
    sharedMemory->cars[childId].lapTime = 0;
}

void generateSectorsTimes(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->cars[childId].sector[i] = generateRandomNumber(sharedMemory, 9, 40);
    }
}

void calculateLapTime(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->cars[childId].lapTime += sharedMemory->cars[childId].sector[i];
    }
}

void calculateTotalTime(sharedMemory *sharedMemory, int childId)
{
    sharedMemory->cars[childId].totalTime += sharedMemory->cars[childId].lapTime;
}

void generatePitStops(sharedMemory *sharedMemory, int childId)
{
    float random = generateRandomNumber(sharedMemory, 1, 0);

    if (random > 0.9)
    {
        sharedMemory->cars[childId].isPitStop = 1;
        sharedMemory->cars[childId].lapTime += generateRandomNumber(sharedMemory, 9, 5);
    }
}

void generateOut(sharedMemory *sharedMemory, int childId, int pId)
{
    float random = generateRandomNumber(sharedMemory, 1, 0);

    if (random > 0.998)
    {
        sharedMemory->cars[childId].isOut = 1;
    }
}

void findBestLap(sharedMemory *sharedMemory, int childId)
{
    if (sharedMemory->cars[childId].lapTime < sharedMemory->cars[childId].bestLap)
    {
        sharedMemory->cars[childId].bestLap = sharedMemory->cars[childId].lapTime;
    }
}

void incrementReaderSem(sharedMemory *sharedMemory)
{
    if (sharedMemory->nbProcessWriting == 20)
    {
        sem_post(&sharedMemory->readerSemaphore);
    }
}
