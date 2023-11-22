#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getValue(char *chaine) {
    // Chercher la position du caractère '|'
    char *separator = strchr(chaine, '|');

    if (separator != NULL) {
        // Utiliser strtok pour diviser la chaîne à partir de '|'
        char *token = strtok(separator, "|");
        
        // Utiliser atoi pour convertir le token suivant '|' en entier
        if (token != NULL) {
            return atoi(token);
        }
    }

    return -1; // Valeur par défaut si le symbole '|' n'est pas trouvé ou s'il n'y a pas de valeur après '|'
}

int main() {
    char chaine[] = "ma chaine |100";
    int val = getValue(chaine);
    printf("la valeur récupérée est : %d\n", val);
    return 0;
}
