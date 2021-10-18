#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct cars
{
    float sector1;
    float sector2;
    float sector3;
} cars;

void main(void)
{
    int carsNumber[] = {1, 2, 3, 4, 5};
    int numberOfCars = sizeof(carsNumber) / 4;
    int positionInCars = 0;
    int pId = 0;

    cars[numberOfCars];

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
