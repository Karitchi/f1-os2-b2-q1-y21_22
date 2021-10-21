typedef struct sharedMemory
{
    int seed;
    int numberOfCarsFinished;
    float allLapTimes[20];
    float bestSectorTimes[3];
    int bestSectorCars[3];
    struct car
    {
        int carNumber;
        float sector[3];
        float lapTime;
        float bestLap;
        int numberOfLaps;
        float totalTime;
        int isPitStop;
        int out;
    } car[20];

} sharedMemory;
