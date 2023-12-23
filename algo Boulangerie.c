//algo Boulangerie
#include <stdio.h>
#include <stdbool.h>

#define NUM_PROCESSES 2

bool choosing[NUM_PROCESSES];
int number[NUM_PROCESSES];

void process(int pid) {
    choosing[pid] = true;

    int max_number = 0;
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        if (number[i] > max_number) {
            max_number = number[i];
        }
    }
    number[pid] = max_number + 1;
    choosing[pid] = false;

    for (int j = 0; j < NUM_PROCESSES; ++j) {
        while (choosing[j]) {
            // Attendre que le processus j termine son choix
        }

        while ((number[j] != 0) && (number[j] < number[pid] || (number[j] == number[pid] && j < pid))) {
            // Attendre que ce soit le tour du processus j
        }
    }

    // Section critique
    printf("Processus %d entre dans la section critique.\n", pid + 1);

    number[pid] = 0; // Réinitialisation du numéro de ticket après la section critique

    printf("Processus %d sort de la section critique.\n", pid + 1);
}

int main() {
    for (int i = 0; i < NUM_PROCESSES; ++i) {
        choosing[i] = false;
        number[i] = 0;
    }

    // Simuler deux processus
    process(0); // Processus 1
    process(1); // Processus 2

    return 0;
}
