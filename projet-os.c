#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct cars
{
    float sector[3];
} cars;

typedef struct shareMemory
{
    cars car[5];
} shareMemory;

void generateChilds(int *pId)
{
    for (int i = 0; i < 5; i++)
    {
        *pId = fork();

        if (!*pId)
        {
            break;
        }
    }
}

int generateRandomNumber(int *seed)
{
    srand(*seed);
    ++*seed;
    return (rand() % 9 + 40);
}

void generateTimesP1(shareMemory *shareMemory, int *seed)
{
    for (int i = 0; i < 3; i++)
    {
        shareMemory->car[0].sector[i] = generateRandomNumber(seed);
    }
}

void main(void)
{
    int carsNumber[] = {1, 2, 3, 4, 5};
    int seed = time(NULL);
    int shmId;
    int pId;

    shareMemory *shareMemory;

    shmId = shmget(1024, sizeof(*shareMemory), IPC_CREAT | 0666);
    shareMemory = shmat(shmId, NULL, 0);

    generateChilds(&pId);
    if (!pId)
    {
        generateTimesP1(shareMemory, &seed);
    }
    else
    {
        printf("%f\n", shareMemory->car[0].sector[0]);
        printf("%f\n", shareMemory->car[0].sector[1]);
        printf("%f\n", shareMemory->car[0].sector[2]);
    }
}
