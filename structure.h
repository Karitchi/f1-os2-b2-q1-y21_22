typedef struct sharedMemory
{
    int seed;
    int numberOfCarsFinished;
    float allLapTimes[20];
    float bestSectors[3];
    struct car
    {
        int carNumber;
        float sector[3];
        float lapTime;
        float bestLap;
        int numberOfLaps;
        float totalTime;
        int pitStops;
    } car[20];

} sharedMemory;
