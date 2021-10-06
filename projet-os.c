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

void generateSectionsTimes(float sector[3][7])
{
    srand(time(NULL));

    printf("%f\n", (float)rand() / (float)(RAND_MAX / 9));
}

void main(void)
{
    pid_t pId = 0;
    int cars[] = {1, 2, 3, 4, 5};
    float sector[3][7];
    int sizeofCars = sizeof(cars) / sizeof(cars[0]);

    generateProccess(&pId, sizeofCars);
    generateSectionsTimes(sector);
}
