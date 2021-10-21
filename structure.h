typedef struct sharedMemory
{
    int seed;
    int numberOfCarsFinished;
    float allLapTimes[20];
    struct car
    {
        int carNumber;
        float sector[3];
        float lapTime;
        float totalTime;
        int pitStops;
    } car[20];

} sharedMemory;
