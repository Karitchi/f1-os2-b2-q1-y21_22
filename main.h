#include "colors.h"

void generateChilds(int *pId, int *childId)
{
    for (int i = 0; i < 20; i++)
    {
        *pId = fork();

        if (!*pId)
        {
            *childId = i;
            break;
        }
    }
}

void findBestSectors(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (sharedMemory->car[i].sector[j] < sharedMemory->car[i + 1].sector[j])
            {
                sharedMemory->bestSectors[j] = sharedMemory->car[i].sector[j];
            }
        }
    }
}

void BubbleSort(sharedMemory *sharedMemory)
{
    int i, j;
    struct car temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sharedMemory->car[j].bestLap > sharedMemory->car[j + 1].bestLap)
            {
                temp = sharedMemory->car[j + 1];
                sharedMemory->car[j + 1] = sharedMemory->car[j];
                sharedMemory->car[j] = temp;
            }
        }
    }
}

void display(sharedMemory *sharedMemory)
{
    system("clear");
    printf("|----------------------------------------------------------------------------------|\n");
    printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | Best lap time  | pit stop(s) |\n");
    for (int i = 0; i < 20; i++)
    {
        if (i == 0)
        {
            printf("|----------------------------------------------------------------------------------|\n");
            printf(GRN "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |     %2d      |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].pitStops);
        }
        else
        {
            printf("|----------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |     %2d      |\n",
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].pitStops);
        }
    }
    printf("|----------------------------------------------------------------------------------|\n\n");
    printf("best sectors 1 : %.3f  2 : %.3f  3 : %.3f\n",
           sharedMemory->bestSectors[0],
           sharedMemory->bestSectors[1],
           sharedMemory->bestSectors[2]);
}