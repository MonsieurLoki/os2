// tutorial: https://www.youtube.com/watch?v=WgVSq-sgHOc&ab_channel=JacobSorber

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_memory.h"
#include <semaphore.h>
#include <fcntl.h>

typedef struct
{
    int id;
    char name[20];
} Person;


char person_name[20];
int person_id;

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

    Person *person;

    // if (argc != 1)
    // {
    //     printf("usage - %s //no args", argv[0]);
    //     return -1;
    // }
    // grab the shared memory block
    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL)
    {
        printf("ERROR: couldn't get block\n");
        return -1;
    }

    person = (Person *) block;

   // Acquire the semaphore before accessing the shared memory
   // not clear if using a semaphore is necessary when reading !?
    sem_wait(sem);

    // printf("Reading: \"%s\"\n", block);

    strncpy(person_name,person->name,20);
    person_id = person->id;

    // Release the semaphore after accessing the shared memory
    sem_post(sem);

    printf("Reading: person ID: %d, person name: %s\n", person_id, person_name);

    detach_memory_block(block);
    return 0;
}