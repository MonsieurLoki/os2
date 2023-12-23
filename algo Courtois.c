//algo Courtois
#include <stdio.h>
#include <stdbool.h>

#define NUM_PROCESSES 2

bool canEnter(int pid, int ticket) {
    // Logique de l'algorithme de Courtois pour déterminer si le processus peut entrer
    if ((ticket % 2) == pid) {
        return true;
    }
    return false;
}

void process(int pid, int ticket) {
    printf("Processus %d veut accéder à la section critique avec le ticket %d.\n", pid + 1, ticket);

    // Vérifier si le processus peut entrer dans la section critique
    if (canEnter(pid, ticket)) {
        printf("Processus %d entre dans la section critique.\n", pid + 1);
        // Effectuer des opérations dans la section critique
        printf("Processus %d sort de la section critique.\n", pid + 1);
    } else {
        printf("Processus %d doit attendre pour accéder à la section critique.\n", pid + 1);
    }
}

int main() {
    // Simuler deux processus avec des tickets différents
    int ticket1 = 5; // Ticket du processus 1 (impair)
    int ticket2 = 6; // Ticket du processus 2 (pair)

    // Processus 1 essaie d'accéder à la section critique
    process(0, ticket1);

    // Processus 2 essaie d'accéder à la section critique
    process(1, ticket2);

    return 0;
}
