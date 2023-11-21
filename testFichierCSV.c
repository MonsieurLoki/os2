// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_LINE_SIZE 1000 // Taille maximale d'une ligne dans le fichier CSV
// #define MAX_COLUMNS 9      // Nombre maximal de colonnes dans une ligne du CSV
// #define MAX_ROWS 100       // Nombre maximal de lignes dans le fichier CSV

// // Structure représentant une ligne du fichier CSV
// typedef struct
// {
//     int course;
//     char ville[MAX_LINE_SIZE];
//     char date[MAX_LINE_SIZE];
//     char pays[MAX_LINE_SIZE];
//     char nom_circuit[MAX_LINE_SIZE];
//     float taille_km;
//     char race[MAX_LINE_SIZE];
//     char circuit[MAX_LINE_SIZE];
//     char date_range[MAX_LINE_SIZE];
// } F1Race;

// int lireEnMemoire(F1Race races[])
// {
//     // variable de type file pour gérer le fichier CSV
//     FILE *file;
//     // buffer pour être sûr qu'on a réservé la place suffisante (MAX_LINE_SIZE) pour être
//     // capable de lire en mémoire toute une ligne du fichier
//     char line[MAX_LINE_SIZE];
//     // le tableau pour contenir l'information du fichier, de façon structurée
//     // chaque élément de ce tableau contiendra une structure de type F1Race, càd
//     // avec les différentes propriété de cette structure
//     // par exemple, races[5].pays contiendra le pays de la course 5
//     // il peut y avoir maximum MAX_ROWS éléments dans ce tableau
//     // F1Race races[MAX_ROWS]; // Tableau de structures F1Race

//     // Ouvrir le fichier CSV en mode lecture
//     file = fopen("listCircuit.csv", "r");
//     // on s'assure que le fichier a été ouvert sans problème
//     if (file == NULL)
//     {
//         printf("Impossible d'ouvrir le fichier.\n");
//         return 1;
//     }

//     // au départ, il y a 0 course de déjà lue
//     int num_races = 0;

//     // Ignorer la première ligne (en-têtes)
//     // on lit dans le buffer line une ligne pour rien
//     fgets(line, MAX_LINE_SIZE, file);

//     // Lire et stocker chaque ligne du fichier CSV dans la structure
//     while (fgets(line, MAX_LINE_SIZE, file) != NULL && num_races < MAX_ROWS)
//     {
//         F1Race race;
//         race.course = -1; // Initialisation

//         // Utilise strtok pour séparer les valeurs par point-virgule
//         char *token = strtok(line, ";");

//         // Stocker chaque valeur dans la structure
//         int column = 0;
//         while (token != NULL && column < MAX_COLUMNS)
//         {
//             switch (column)
//             {
//             case 0:
//                 // Convertit la chaîne de caractères token en un entier à l'aide de la fonction atoi() et stocke la valeur dans le champ course de la structure F1Race
//                 race.course = atoi(token);
//                 break;
//             case 1:
//                 // Copie la chaîne de caractères token dans les champs correspondants de la structure F1Race (ville, date, pays, etc.) en utilisant strcpy() pour la copie.
//                 strcpy(race.ville, token);
//                 break;
//             case 2:
//                 strcpy(race.date, token);
//                 break;
//             case 3:
//                 strcpy(race.pays, token);
//                 break;
//             case 4:
//                 strcpy(race.nom_circuit, token);
//                 break;
//             case 5:
//                 // Convertit la chaîne de caractères token en un nombre flottant (à virgule flottante) à l'aide de la fonction atof() et stocke la valeur dans le champ taille_km de la structure F1Race
//                 race.taille_km = atof(token);
//                 break;
//             case 6:
//                 strcpy(race.race, token);
//                 break;
//             case 7:
//                 strcpy(race.circuit, token);
//                 break;
//             case 8:
//                 strcpy(race.date_range, token);
//                 break;
//             }
//             // trouve le prochain morceau de texte dans la chaîne, en utilisant le point-virgule comme séparateur, pour une découpe ultérieure.
//             // La fonction strtok est utilisée pour découper une chaîne de caractères en "tokens" (morceaux) basés sur un délimiteur spécifié.
//             token = strtok(NULL, ";");
//             column++;
//         }

//         // Stocker la ligne dans le tableau de structures F1Race
//         // stocke la structure de données représentant une course dans le tableau races, puis incrémente le compteur pour la prochaine course à enregistrer.
//         // voici en plusieurs étapes : F1Race tempRace = race;
//         //    races[num_races] = tempRace;
//         //    num_races++;
//         races[num_races++] = race;
//     }

//     // Fermer le fichier
//     fclose(file);
//     return num_races;
// }
// int afficherCircuits(F1Race races[], int num_races)
// {
//     // Exemple d'utilisation : Affichage des données de chaque course
//     for (int i = 0; i < num_races; ++i)
//     {
//         printf("Course : %d\nVille : %s\nDate : %s\nPays : %s\nNom du circuit : %s\nTaille (km) : %.3f\nRace : %s\nCircuit : %s\nDate Range : %s\n\n",
//                races[i].course, races[i].ville, races[i].date, races[i].pays, races[i].nom_circuit, races[i].taille_km,
//                races[i].race, races[i].circuit, races[i].date_range);
//     }

//     return 0;
// }

// int main()
// {
//     F1Race races[MAX_ROWS];
//     int num_races = lireEnMemoire(races);
//     afficherCircuits(races, num_races);
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1000 // Taille maximale d'une ligne dans le fichier CSV
#define MAX_COLUMNS 9      // Nombre maximal de colonnes dans une ligne du CSV
#define MAX_ROWS 100       // Nombre maximal de lignes dans le fichier CSV

// Structure représentant une ligne du fichier CSV
typedef struct
{
    int course;
    char ville[MAX_LINE_SIZE];
    char date[MAX_LINE_SIZE];
    char pays[MAX_LINE_SIZE];
    char nom_circuit[MAX_LINE_SIZE];
    float taille_km;
    char race[MAX_LINE_SIZE];
    char circuit[MAX_LINE_SIZE];
    char date_range[MAX_LINE_SIZE];
} F1Race;

int lireEnMemoire(F1Race races[])
{
    // variable de type file pour gérer le fichier CSV
    FILE *file;
    // buffer pour être sûr qu'on a réservé la place suffisante (MAX_LINE_SIZE) pour être
    // capable de lire en mémoire toute une ligne du fichier
    char line[MAX_LINE_SIZE];
    // le tableau pour contenir l'information du fichier, de façon structurée
    // chaque élément de ce tableau contiendra une structure de type F1Race, càd
    // avec les différentes propriété de cette structure
    // par exemple, races[5].pays contiendra le pays de la course 5
    // il peut y avoir maximum MAX_ROWS éléments dans ce tableau
    // F1Race races[MAX_ROWS]; // Tableau de structures F1Race

    // Ouvrir le fichier CSV en mode lecture
    file = fopen("listCircuit.csv", "r");
    // on s'assure que le fichier a été ouvert sans problème
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    // au départ, il y a 0 course de déjà lue
    int num_races = 0;

    // Ignorer la première ligne (en-têtes)
    // on lit dans le buffer line une ligne pour rien
    fgets(line, MAX_LINE_SIZE, file);

    // Lire et stocker chaque ligne du fichier CSV dans la structure
    while (fgets(line, MAX_LINE_SIZE, file) != NULL && num_races < MAX_ROWS)
    {
        F1Race race;
        race.course = -1; // Initialisation

        // Utilise strtok pour séparer les valeurs par point-virgule
        char *token = strtok(line, ";");

        // Stocker chaque valeur dans la structure
        int column = 0;
        while (token != NULL && column < MAX_COLUMNS)
        {
            switch (column)
            {
            case 0:
                // Convertit la chaîne de caractères token en un entier à l'aide de la fonction atoi() et stocke la valeur dans le champ course de la structure F1Race
                race.course = atoi(token);
                break;
            case 1:
                // Copie la chaîne de caractères token dans les champs correspondants de la structure F1Race (ville, date, pays, etc.) en utilisant strcpy() pour la copie.
                strcpy(race.ville, token);
                break;
            case 2:
                strcpy(race.date, token);
                break;
            case 3:
                strcpy(race.pays, token);
                break;
            case 4:
                strcpy(race.nom_circuit, token);
                break;
            case 5:
                // Convertit la chaîne de caractères token en un nombre flottant (à virgule flottante) à l'aide de la fonction atof() et stocke la valeur dans le champ taille_km de la structure F1Race
                race.taille_km = atof(token);
                break;
            case 6:
                strcpy(race.race, token);
                break;
            case 7:
                strcpy(race.circuit, token);
                break;
            case 8:
                strcpy(race.date_range, token);
                break;
            }
            // trouve le prochain morceau de texte dans la chaîne, en utilisant le point-virgule comme séparateur, pour une découpe ultérieure.
            // La fonction strtok est utilisée pour découper une chaîne de caractères en "tokens" (morceaux) basés sur un délimiteur spécifié.
            token = strtok(NULL, ";");
            column++;
        }

        // Stocker la ligne dans le tableau de structures F1Race
        // stocke la structure de données représentant une course dans le tableau races, puis incrémente le compteur pour la prochaine course à enregistrer.
        // voici en plusieurs étapes : F1Race tempRace = race;
        //    races[num_races] = tempRace;
        //    num_races++;
        races[num_races++] = race;
    }

    // Fermer le fichier
    fclose(file);
    return num_races;
}
int afficherCircuits(F1Race races[], int num_races)
{
    // Affichage des en-têtes du tableau avec des largeurs de champ fixes
    printf("%-6s| %-18s| %-8s| %-20s| %-35s| %-11s| %-25s| %-30s| %-12s\n",
           "Course", "Ville", "Date", "Pays", "Nom du circuit", "Taille (km)", "Race", "Circuit", "Date Range");

    // Affichage des séparateurs
    printf("------|-------------------|---------|---------------------|------------------------------------|------------|--------------------------|-------------------------------|--------------\n");

    // Affichage des données de chaque course
    for (int i = 0; i < num_races; ++i)
    {
        printf("%-6d| %-18s| %-8s| %-20s| %-35s| %-11.3f| %-25s| %-30s| %s |\n",
               races[i].course, races[i].ville, races[i].date, races[i].pays, races[i].nom_circuit, races[i].taille_km,races[i].race, races[i].circuit, races[i].date_range);
    }

    return 0;
}

int main()
{
    F1Race races[MAX_ROWS];
    int num_races = lireEnMemoire(races);
    afficherCircuits(races, num_races);
}
