#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main(void)
{
    int pId = 0;
    int cars[] = {1, 2, 3, 4, 5, 6, 7};
    float sector[3][7];

    srand(time(NULL));

    // Generation des temps de chaque voiture pour chaque secteur
    for (int i = 0; i < sizeof(cars) / 4; i++)
    {
        pId = fork();
    }
    printf("%d /n", pId);
}
