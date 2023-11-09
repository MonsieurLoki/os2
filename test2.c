// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>

// //code avec un processus fils qui envoie un message au parent

// int main() {
//     int fd[2]; //file descriptor de deux éléments, fd[0](lecture) et fd[1](écriture) sont les deux éléments du tableau.
//     pid_t pid;

//     if (pipe(fd) == -1) {
//         perror("Erreur lors de la création du tube");
//         exit(EXIT_FAILURE);
//     }

//     pid = fork();

//     if (pid == -1) {
//         perror("Erreur lors de la création du processus fils");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0) {
//         // Code du processus fils
//         close(fd[0]); //on ferme d'abord le fd de lecture car l'enfant n'a pas l'intention de lire quelque chose dans le tube et va donc d'abord le fermer

//         sleep(3);

//         char message[] = "je suis le fils numéro un";
//         write(fd[1], message, strlen(message) + 1);

//         close(fd[1]);

//         exit(EXIT_SUCCESS);
//     } else {
//         // Code du processus parent
//         close(fd[1]); //on ferme d'abord le fd d'écriture car le parent n'a pas l'intention d'écrire quelque chose dans le tube et va donc d'abord le fermer

//         char buffer[100]; // le buffer va lire les données dans le tube reçu par l'enfant, je lui ai donné une taille de 100 charactères.
//         ssize_t nbrDeChar = read(fd[0], buffer, sizeof(buffer));
//         if (nbrDeChar > 0) {
//             printf("Message reçu du fils : %s\n", buffer);
//         } else {
//             perror("Erreur lors de la lecture du tube");
//         }

//         close(fd[0]); //et on ferme le fd de lecture car il n'y a plus rien à lire.

//         exit(EXIT_SUCCESS);
//     }

//     return 0;
// }

// code avec un processus fils qui envoie un message au parent
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>

// int main()
// {
//     int fd[2];
//     pid_t pid;

//     if (pipe(fd) == -1)
//     {
//         perror("Erreur lors de la création du tube");
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; i < 2; i++)
//     {

//         pid = fork();

//         if (pid == -1)
//         {
//             perror("Erreur lors de la création du processus fils");
//             exit(EXIT_FAILURE);
//         }

//         if (pid == 0)
//         {
//             // Code du processus fils
//             close(fd[0]);

//             sleep(3);

//             char message[50];
//             snprintf(message, sizeof(message), "je suis le fils numéro %d", i); // snprintf recommandée pour formater des chaînes de caractères
//             write(fd[1], message, strlen(message) + 1);                         // Écrit le message dans le tube

//             close(fd[1]);

//             exit(EXIT_SUCCESS);
//         }
//     }

//     // Code du processus parent
//     close(fd[1]);

//     for (int i = 1; i <= 2; i++)
//     {
//         char buffer[100];
//         ssize_t nbrDeChar = read(fd[0], buffer, sizeof(buffer)); // Lit le message depuis le tube et nbrDeChar vaut le nombre d'octets trouvé dans le message.
//         if (nbrDeChar > 0)
//         {
//             printf("Message reçu du fils : %s\n", buffer);
//         }
//         else
//         {
//             perror("Erreur lors de la lecture du tube");
//         }
//     }

//     close(fd[0]);

//     exit(EXIT_SUCCESS);
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    int fd1[2], fd2[2];
    pid_t pid1, pid2;

    if (pipe(fd1) == -1)
    {
        perror("Erreur lors de la création du tube");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();

    if (pid1 == -1)
    {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {
        // Code du premier processus fils
        close(fd1[0]);

        sleep(6);

        char message[50];
        snprintf(message, sizeof(message), "je suis le fils numéro %d", 1);
        write(fd1[1], message, strlen(message) + 1);

        close(fd1[1]);

        exit(EXIT_SUCCESS);
    }
    else
    {
        // Code du processus parent
            if (pipe(fd2) == -1)
    {
        perror("Erreur lors de la création du tube");
        exit(EXIT_FAILURE);
    }
        pid2 = fork();

        if (pid2 == -1)
        {
            perror("Erreur lors de la création du second processus fils");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0)
        {
            // Code du second processus fils
            close(fd2[0]);

            sleep(3);

            char message[50];
            snprintf(message, sizeof(message), "je suis le fils numéro %d", 2);
            write(fd2[1], message, strlen(message) + 1);

            close(fd2[1]);

            exit(EXIT_SUCCESS);
        }
        else
        {
            // Code du processus parent
            close(fd2[1]);

            char buffer[50];
            read(fd1[0], buffer, sizeof(buffer));
            printf("%s\n", buffer);

            read(fd2[0], buffer, sizeof(buffer));
            printf("%s\n", buffer);

            close(fd2[0]);
        }
    }

    return 0;
}
