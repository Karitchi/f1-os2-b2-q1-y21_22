#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// genere un processus pour chaque voiture.
void generateProccess(pid_t *pId, int sizeofCars)
{
    for (int i = 0; i < sizeofCars; i++)
    {
        *pId = fork();
        if (*pId > 0)
        {
            break;
        }
    }
}

void generateSectionsTimes(float sector[7][3], int *sectorHeadX)
{
    wait();
    srand(time(NULL));
    for (int i = 0; i < 3; i++)
    {
        sector[*sectorHeadX][i] = (float)rand() / (float)(RAND_MAX / 9);
        printf("%f %d\n", sector[*sectorHeadX][i], *sectorHeadX);
    }
    *sectorHeadX++;
}

void main(void)
{
    pid_t pId = 0;
    int cars[] = {1, 2, 3, 4, 5};
    float sector[7][3];
    int sectorHeadX = 0;
    int sizeofCars = sizeof(cars) / sizeof(cars[0]);

    generateProccess(&pId, sizeofCars);
    if (pId > 0)
    {
        generateSectionsTimes(sector, &sectorHeadX);
    }
    if (pId == 0)
    {
        // printf("asd : %f\n", sector[0][0]);
    }
}