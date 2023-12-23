//algo Morice
#include <stdio.h>
#include <stdbool.h>

#define NUM_PROCESSES 3

bool attente_jeton[NUM_PROCESSES];
int jeton;

void process(int pid) {
    // Demande d'accès à la ressource partagée
    if (jeton == pid) {
        printf("Processus %d accède à la ressource partagée.\n", pid + 1);
        // Utilisation de la ressource

        // Transfert du jeton au processus suivant dans l'ordre circulaire
        jeton = (pid + 1) % NUM_PROCESSES;
    } else {
        printf("Processus %d attend le jeton pour accéder à la ressource.\n", pid + 1);
        attente_jeton[pid] = true;

        while (jeton != pid) {
            // Attente du jeton
        }
        attente_jeton[pid] = false;
        printf("Processus %d reçoit le jeton et accède à la ressource.\n", pid + 1);
        // Utilisation de la ressource

        // Transfert du jeton au processus suivant dans l'ordre circulaire
        jeton = (pid + 1) % NUM_PROCESSES;
    }
}

int main() {
    jeton = 0; // Initialisation du jeton avec le premier processus

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        attente_jeton[i] = false;
    }

    // Simulation de l'accès à la ressource par les processus
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        process(i);
    }

    return 0;
}
