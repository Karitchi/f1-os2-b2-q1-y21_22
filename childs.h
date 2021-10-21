float generateRandomNumber(sharedMemory *sharedMemory, int max, int added)
{

    srand(sharedMemory->seed);
    sharedMemory->seed++;
    return (float)rand() / (float)(RAND_MAX)*max + added;
}

void generateTimesP1(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].sector[i] = generateRandomNumber(sharedMemory, 9, 40);
    }
}

void calculateLapTime(sharedMemory *sharedMemory, int childId)
{
    sharedMemory->car[childId].lapTime = 0;

    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].lapTime += sharedMemory->car[childId].sector[i];
    }
}

void calculateTotalTime(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].totalTime += sharedMemory->car[childId].sector[i];
    }
}

void generatePitStops(sharedMemory *sharedMemory, int childId)
{
    float random = generateRandomNumber(sharedMemory, 1, 0);

    if (random > 0.9)
    {
        sharedMemory->car[childId].pitStops++;
        sharedMemory->car[childId].lapTime += generateRandomNumber(sharedMemory, 9, 5);
    }
}