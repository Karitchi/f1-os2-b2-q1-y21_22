#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
//#include "resources.c"

/*
* @id (int) - id of vehicle
* @time[3] (float) - random number [s1, s2, s3]
* @out[3] (int) - false(0) by default, true(1) if session abanoned [bool, bool, bool]
* @stand[3][2] (int) - false(0) by default, true(1) if in stand [[bool, return time], [bool, return time], [bool, return time]]
*/
struct vehicle
{
    int id;
    float time[3];
    int out[3];
    int stand[3][2];
};

/*
* receves struct and initializez vehicles with id, time(as float random, min 30 max 39.99) out(0 by default), standout(0 by default)
* 
* ! WHEN PRINTING THE VAUE OF TIME PRINT AS "%.2f"
*/
void randomFloat(struct vehicle cars[20]){

    int idCars[20] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

    srand((unsigned int)time(NULL));

    for(int i = 0; i < 20; i++){

        // generate random float from 0 to 10
        float max = 9.99;
        float randomFloat = ((float)rand()/(float)(RAND_MAX)) * max;

        // += 30 to random float
        float totalTime = randomFloat + 30;

        // initialize 
        cars[i].id = idCars[i];

        for (int j = 0; j < 3; j++)
        {
            cars[i].time[j] = totalTime;
            cars[i].out[j] = 0;
            for (int n = 0; n < 2; n++)
            {
                cars[i].stand[j][n] = 0;
            }
            
        }

    }
};

/*
* f() generate out, rand number between 1 - 5
* change state of out in a random sector to a random vehicle
*/

/*
* f() generate stand, rand number, in rand section
* 
*/

/*
* f() sort times 
* store in new array containing id's
* array[20][2]  => [[id, time difference with previous]...]
*/


int main(){
    struct vehicle cars[20];

    randomFloat(cars);
    

    printf("%d %.2f %d %d \n", cars[0].id, cars[0].time[0], cars[0].out[0], cars[0].stand[0][0]);
}