typedef struct sharedMemory
{
    int seed;
    int numberOfCarsFinished;
    int counterForCarPosition;
    float allLapTimes[20];
    float bestSectorTimes[3];
    int bestSectorCars[3];
    struct cars
    {
        int carNumber;
        float sector[3];
        float lapTime;
        int numberOfLaps;
        float bestLap;
        float totalTime;
        float avgSpeed;
        float numberOfSecondsAhead;
        int isPitStop;
        int isOut;
        int isEliminated;
    } cars[20];
} sharedMemory;