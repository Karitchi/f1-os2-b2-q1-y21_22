typedef struct shareMemory
{
    struct car
    {
        int carNumber;
        float sector[3];
        float lapTime;
        float timeSpent;
    } car[20];

    int seed;
} shareMemory;