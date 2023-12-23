// tutorial: https://www.youtube.com/watch?v=WgVSq-sgHOc&ab_channel=JacobSorber

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_memory.h"
#include <semaphore.h>
#include <fcntl.h>

#define BLOCK SIZE 4096

typedef struct
{
    int id;
    char name[20];
} Person;

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

    if (argc != 2)
    {
        printf("usage - %s [stuff to write]", argv[0]);
        return -1;
    }
    // grab the shared memory block

    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    // Person *person = (Person *) attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL)
    {
        printf("ERROR: couldn't get block\n");
        return -1;
    }

    // printf("Writing: \"%s\"\n", argv[1]);
    printf("Writing: the person data in memory\n");

    person = (Person *)block;

  // Acquire the semaphore before accessing the shared memory
    sem_wait(sem);

    // strncpy(block, argv[1], BLOCK_SIZE);
    // Write data to the shared memory segment
    person->id = 1;
    strcpy(person->name, "John");

    // Release the semaphore after accessing the shared memory
    sem_post(sem);

    detach_memory_block(block);

    sem_close(sem);
    sem_unlink("my_semaphore");

    return 0;
}
