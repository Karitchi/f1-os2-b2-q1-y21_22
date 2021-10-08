#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
// #include <sys/wait.h>
// #include <sys/types.h>

typedef struct car
{
    float sector1;
    float sector2;
    float sector3;
} structCar;

void generateChilds(int numberOfCars, int *pId)
{
    for (int i = 0; i < numberOfCars; i++)
    {
        *pId = fork();
        if (!*pId)
        {
            break;
        }
    }
}

void generateRandomTimes(structCar *cars, int *positionInCars)
{
    wait();
    srand(time(NULL));

    cars[*positionInCars].sector1 = ((35 - 45) * ((float)rand() / RAND_MAX)) + 35;
    cars[*positionInCars].sector2 = ((30 - 40) * ((float)rand() / RAND_MAX)) + 30;
    cars[*positionInCars].sector3 = ((32 - 48) * ((float)rand() / RAND_MAX)) + 32;

    *positionInCars++;
}

void printSectors(structCar *cars, int numberOfCars)
{
    for (int i = 0; i < numberOfCars; i++)
    {
        printf("voiture %d :\n\t-secteur1 : %f\n\t-secteur2 : %f\n\t-secteur3 : %f\n\n",
               i + 1, cars[i].sector1, cars[i].sector2, cars[i].sector3);
    }
}

void main(void)
{
    int carsNumber[] = {1, 2, 3, 4, 5};
    int numberOfCars = sizeof(carsNumber) / 4;
    int positionInCars = 0;
    int pId = 0;
    structCar cars[numberOfCars];

    generateChilds(numberOfCars, &pId);
    if (!pId)
    {
        generateRandomTimes(cars, &positionInCars);
    }

    if (pId)
    {
        printSectors(cars, numberOfCars);
    }
}
