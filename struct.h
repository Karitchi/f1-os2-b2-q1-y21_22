typedef struct sharedMemory
{
    sem_t readerSemaphore;
    sem_t writerSemaphore;
    int seed;
    int numberOfCarsFinished;
    int counterForCarPosition;
    float allLapTimes[20];
    float bestSectorTimes[3];
    int bestSectorCars[3];
    int nbProcessWriting;

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
        float interval;
    } cars[20];
} sharedMemory;