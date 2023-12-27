// test pipes bloquants
// ps -aux | grep "myprocess"
// echo   $(ps -aux | grep "pipeBlo" | awk '{ print $2 }')
// kill   $(ps -aux | grep "pipeBlo" | awk '{ print $2 }')
// tutorial: https://www.youtube.com/watch?v=WgVSq-sgHOc&ab_channel=JacobSorber

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared_memory.h"

#define IPC_RESULT_ERROR (-1)
#define NB_VOITURE 4

#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

// attach a shared memory block
// associated with filename
// create it if it doesn't exist

// all of the programs will share these values
#define BLOCK_SIZE 4096
#define FILENAME "cars.c"

#endif

// definition of the car structure
struct Car
{
    int car_nr;
    char pilote[20];
    float best_time_sector1;
    float best_time_sector2;
    float best_time_sector3;
    int position;
    int secteur_courant;
};

// definition of the Best_sector times
struct Best_sector
{
    // int time;      // best time for this sector in secs
    int time_ms;   // best time for this sector in ms
    int car_index; // car having done the best time for this sector
};

// definition of the structure of the shared memmory
typedef struct
{
    struct Car cars[NB_VOITURE + 1];
    struct Best_sector best_sectors[3];
} Shmem_data;

Shmem_data *shmem_data;

// shared memomry
static int get_shared_block(char *filename, int size)
{
    key_t key;
    // Request a key
    // The key is linked to a filename, so that other programs can access it.
    key = ftok(filename, 0);
    if (key == IPC_RESULT_ERROR)
    {
        return IPC_RESULT_ERROR;
    }
    // get shared block create it if it doesn't exist
    return shmget(key, size, 0644 | IPC_CREAT);
}

void *attach_memory_block(char *filename, int size)
{
    int shared_block_id = get_shared_block(filename, size);
    void *result;
    if (shared_block_id == IPC_RESULT_ERROR)
    {
        return NULL;
    }
    // map the shared block into this process's memory
    // and give me a pointer to it
    result = shmat(shared_block_id, NULL, 0);
    if (result == (char *)IPC_RESULT_ERROR)
    {
        return NULL;
    }
    return result;
}

bool detach_memory_block(char *block)
{
    return (shmdt(block) != IPC_RESULT_ERROR);
}

bool destroy_memory_block(char *filename)
{
    int shared_block_id = get_shared_block(filename, 0);
    if (shared_block_id == IPC_RESULT_ERROR)
    {
        return NULL;
    }
    return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

void destroy_shmem()
{
    if (destroy_memory_block(FILENAME))
    {
        printf("Destroyed block: %s\n", FILENAME);
    }
    else
    {
        printf("Could not destroy block: %s\n", FILENAME);
    }
}

int myRandom(int min, int max)
{
    int nombreAlea = rand() % (max - min + 1) + min;
    return nombreAlea;
}

void gererSecteur(int car_nr, int nr_secteur)
{
    int dureeCourse_ms = myRandom(7000, 12000);
    int passed_ms = 0; // nb of ms passed since beginning of the sector
    int delay_ms = 100;
    int position;
    shmem_data->cars[car_nr].secteur_courant = nr_secteur;
    while (passed_ms <= dureeCourse_ms)
    {
        position = ((100 * passed_ms) / dureeCourse_ms);
        shmem_data->cars[car_nr].position = position;
        usleep(delay_ms * 1000); // attendre x microsecond
        passed_ms += delay_ms;
    }
    shmem_data->cars[car_nr].position = 100;
}

void dessiner(int car_nr, int position)
{
    int secteur_courant = shmem_data->cars[car_nr].secteur_courant;
    const int nombreTirets = 30;
    char secteur_0[nombreTirets + 1];
    char secteur_1[nombreTirets + 1];
    char secteur_2[nombreTirets + 1];

    // fill sectors
    for (int i = 0; i < nombreTirets; i++)
    {
        secteur_0[i] = '-';
    }
    secteur_0[nombreTirets] = '\0';
    secteur_0[nombreTirets - 1] = '|';

    for (int i = 0; i < nombreTirets; i++)
    {
        secteur_1[i] = '-';
    }
    secteur_1[nombreTirets] = '\0';
    secteur_1[nombreTirets - 1] = '|';

    for (int i = 0; i < nombreTirets; i++)
    {
        secteur_2[i] = '-';
    }
    secteur_2[nombreTirets] = '\0';
    secteur_2[nombreTirets - 1] = '|';

    // compute where to put the car
    int positionEtoile = (nombreTirets - 1) * position / 100;
    if (secteur_courant == 0)
    {
        secteur_0[positionEtoile] = '*';
    }
    if (secteur_courant == 1)
    {
        secteur_1[positionEtoile] = '*';
    }
    if (secteur_courant == 2)
    {
        secteur_2[positionEtoile] = '*';
    }

    printf("%3d) voiture sect:%2d, pos:%3d ", car_nr, secteur_courant, position);

    printf("%s%s%s\n", secteur_0, secteur_1, secteur_2);
}

// prend la valeur a la fin de la chaine de caractère après la |
int getValue(char *chaine)
{
    // Chercher la position du caractère '|'
    char *separator = strchr(chaine, '|');

    if (separator != NULL)
    {
        // Utiliser strtok pour diviser la chaîne à partir de '|'
        char *token = strtok(separator, "|");

        // Utiliser atoi pour convertir le token suivant '|' en entier
        if (token != NULL)
        {
            return atoi(token);
        }
    }

    return -1; // Valeur par défaut si le symbole '|' n'est pas trouvé ou s'il n'y a pas de valeur après '|'
}

void vivre_ma_vie_de_voiture(int car_nr)
{
    char *block;
    block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL)
    {
        printf("ERROR: couldn't get block\n");
        exit(1);
    }

    shmem_data = (Shmem_data *)block;

    srand(time(NULL) + car_nr);

    gererSecteur(car_nr, 0);
    gererSecteur(car_nr, 1);
    gererSecteur(car_nr, 2);

    // int i = 0;
    // int delay = 1;

    detach_memory_block(block);
}

// cette fonction renvoit 1 si toutes les voitures ont terminé, c'est-à-dire que le position est à 100% du secteur
// et renvoit 0 autrement
int toutesLesVoituresOntTermine()
{
    for (int i = 0; i < NB_VOITURE; i++)
    {
        if (shmem_data->cars[i].position == 100)
        {
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

void gererVoitures()
{
    int iter = 0;
    while (1)
    {
        printf("\n");
        iter++;

        for (int i = 0; i < NB_VOITURE; i++)
        {
            printf("%3d) voiture %2d, secteur courant : %d, position :  %3d \n", iter, i, shmem_data->cars[i].secteur_courant, shmem_data->cars[i].position);
        }

        for (int i = 0; i < NB_VOITURE; i++)
        {
            dessiner(i, shmem_data->cars[i].position);
        }

        if (toutesLesVoituresOntTermine() == 1)
        {
            break;
        }
        usleep(100 * 1000); // attendre x microsecond
    }
}

int lancerVoiture(int nr_voiture)
{
    int pid;
    pid = fork();

    if (pid == -1)
    {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        vivre_ma_vie_de_voiture(nr_voiture);
        exit(0);
    }
    else
    {
        return pid;
    }
}

int main()
{
    destroy_shmem();

    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL)
    {
        printf("ERROR: couldn't get block\n");
        exit(1);
    }

    // mapping the Shmem_data structure on the shared memory block
    shmem_data = (Shmem_data *)block;

    pid_t pid[NB_VOITURE];
    for (int i = 0; i < NB_VOITURE; i++)
    {
        pid[i] = lancerVoiture(i);
    }
    gererVoitures();
    detach_memory_block(block);

    destroy_shmem();

    return 0;
}
