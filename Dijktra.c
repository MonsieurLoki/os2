//dijktra
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define NUM_NODES 6

// Fonction pour trouver le nœud non visité avec la distance minimale
int minDistance(int dist[], bool visited[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < NUM_NODES; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Fonction pour afficher les distances minimales depuis le nœud source
void printSolution(int dist[]) {
    printf("Nœud \t Distance depuis le nœud source\n");
    for (int i = 0; i < NUM_NODES; i++) {
        printf("%d \t %d\n", i, dist[i]);
    }
}

// Fonction implémentant l'algorithme de Dijkstra
void dijkstra(int graph[NUM_NODES][NUM_NODES], int src) {
    int dist[NUM_NODES];    // Tableau pour enregistrer les distances minimales
    bool visited[NUM_NODES];    // Tableau pour marquer les nœuds visités

    // Initialisation de toutes les distances comme infini et les nœuds comme non visités
    for (int i = 0; i < NUM_NODES; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }

    // Distance du nœud source à lui-même est toujours 0
    dist[src] = 0;

    // Trouver les distances minimales pour tous les nœuds
    for (int count = 0; count < NUM_NODES - 1; count++) {
        int u = minDistance(dist, visited);
        visited[u] = true;

        // Mise à jour des distances des nœuds adjacents du nœud choisi
        for (int v = 0; v < NUM_NODES; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Afficher les distances minimales depuis le nœud source
    printSolution(dist);
}

int main() {
    // Graphe représenté sous forme de matrice d'adjacence
    int graph[NUM_NODES][NUM_NODES] = {
        {0, 7, 0, 0, 3, 0},
        {7, 0, 2, 0, 0, 0},
        {0, 2, 0, 1, 0, 4},
        {0, 0, 1, 0, 2, 0},
        {3, 0, 0, 2, 0, 0},
        {0, 0, 4, 0, 0, 0}
    };

    int source = 0; // Nœud source

    printf("Les distances minimales depuis le nœud source %d sont :\n", source);
    dijkstra(graph, source);

    return 0;
}
