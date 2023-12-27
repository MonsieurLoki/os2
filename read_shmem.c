// tutorial: https://www.youtube.com/watch?v=WgVSq-sgHOc&ab_channel=JacobSorber

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_memory.h"
#include <semaphore.h>
#include <fcntl.h>

typedef struct
{
    int car_nr;
    char pilote[20];
    float best_time_sector1;
    float best_time_sector2;
    float best_time_sector3;
} Car;

sem_t semaphore;

int main(int argc, char *argv[])
{

    sem_t *sem;
    // Create a semaphore with an initial value of 1
    sem = sem_open("my_semaphore", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    Car *car;
    int car_nr;
    char pilote[20];
    float best_sector1;
    float best_sector2;
    float best_sector3;

    // grab the shared memory block
    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL)
    {
        printf("ERROR: couldn't get block\n");
        exit(1);
    }

    car = (Car *)block;

    // Acquire the semaphore before accessing the shared memory
    // not clear if using a semaphore is necessary when reading !?
    sem_wait(sem);

    car_nr = car->car_nr;
    strncpy(pilote, car->pilote, 20);
    best_sector1 = car->best_time_sector1;
    best_sector2 = car->best_time_sector2;
    best_sector3 = car->best_time_sector3;
    // printf("voici ce que j'ai trouvé dans la mém partagée : %s\n", block);

    // Release the semaphore after accessing the shared memory
    sem_post(sem);

    printf("On vient juste de lire dans la mémoire partagée: car_nr: %d, pilote: %s, best1: %f, best2: %f, best3: %f\n", car_nr, pilote,
           best_sector1, best_sector2, best_sector3);

    detach_memory_block(block);
    sem_close(sem);
    sem_unlink("my_semaphore");

    return 0;
}