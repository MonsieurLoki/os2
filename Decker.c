//decker
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 

#define NUM_PROCESSES 2

int turn[NUM_PROCESSES];
bool flag[NUM_PROCESSES];

void process(int pid) {
    int other = 1 - pid; // Autre processus

    // Entrée dans la section critique
    flag[pid] = true;
    turn[pid] = pid;

    // Attente du tour ou du relâchement de l'autre processus
    while (flag[other] && turn[pid] == pid) {
        // Attendre
    }

    // Section critique
    printf("Process %d entre dans la section critique.\n", pid + 1);
    sleep(1); // Simulation d'une opération
    printf("Process %d sort de la section critique.\n", pid + 1);

    // Sortie de la section critique
    flag[pid] = false;
}

int main() {
    // Initialisation des variables partagées
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        turn[i] = 0;
        flag[i] = false;
    }

    // Création des processus
    pid_t pid;
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        pid = fork();

        if (pid < 0) {
            printf("Erreur lors de la création du processus.\n");
            return 1;
        } else if (pid == 0) {
            process(i); // Processus enfant
            return 0;
        }
    }

    // Attente de la fin des processus enfants
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        wait(NULL);
    }

    return 0;
}
