typedef struct sharedMemory
{
    int seed;
    int numberOfCarsFinished;
    int counterForCarPosition;
    float allLapTimes[20];
    float bestSectorTimes[3];
    int bestSectorCars[3];
    struct car
    {
        int carNumber;
        float sector[3];
        float lapTime;
        float bestLap;
        float totalTime;
        int isPitStop;
        int isOut;
        int isEliminated;
    } car[20];

} sharedMemory;