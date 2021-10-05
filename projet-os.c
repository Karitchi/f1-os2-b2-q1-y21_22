#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main(void)
{
    int cars[] = {1, 2, 3, 4, 5, 6, 7};
    float sector[3][7];

    srand(time(NULL));

    // Generation des temps de chaque voiture pour chaque secteur
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < sizeof(cars) / 4; j++)
        {
            sector[i][j] = ((rand() / (float)(RAND_MAX)) * 9.0) + 35;
            printf("%f ", sector[i][j]);
        }
        printf("\n");
    }
}


