// test pipes bloquants
// ps -aux | grep "myprocess"
// echo   $(ps -aux | grep "pipeBlo" | awk '{ print $2 }')
// kill   $(ps -aux | grep "pipeBlo" | awk '{ print $2 }')

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

int myRandom(int min, int max)
{
    int nombreAlea = rand() % (max - min + 1) + min;
    return nombreAlea;
}

int myFct(char msg[20], char buffer[150])
{
    printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg, buffer);
}

int fctParent(int numPipe[])
{
    // Code du processus parent
    close(numPipe[1]); // Fermer le descripteur d'écriture, car le parent lit seulement
    char buffer[150];
    int bytesRead;
    //  int status;
    //         waitpid(pid, &status, 0); // Attendre que le processus fils se termine
    fcntl(numPipe[0], F_SETFL, O_NONBLOCK);
    int k = 0;
    while (1)
    {
        k++;
        // Lecture non bloquante
        bytesRead = read(numPipe[0], buffer, sizeof(buffer));
        if (bytesRead > 0)
        {
            char msg1[20] = "bonjour";
            char msg2[20] = "au revoir";
            char msg3[20] = "a demain";
            k = 0;
            if (strstr(buffer, msg1) != NULL)
            {
                // printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg1, buffer);
                myFct(msg1, buffer);
            }
            // else
            // {
            //     printf("Le mot '%s' et '%s' n'ont pas été trouvé dans la phrase : '%s'\n", msg2, msg3, buffer);
            // }
            else if (strstr(buffer, msg2) != NULL)
            {
                // printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg2, buffer);
                myFct(msg2, buffer);
            }
            // else
            // {
            //     printf("Le mot '%s' et '%s' n'ont pas été trouvé dans la phrase : '%s'\n", msg1, msg3, buffer);
            // }
            else if (strstr(buffer, msg3) != NULL)
            {
                // printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg3, buffer);
                myFct(msg3, buffer);
            }
            // else
            // {
            //     printf("Le mot '%s' et '%s' n'ont pas été trouvé dans la phrase : '%s'\n", msg1, msg2, buffer);
            // }
            // printf("Message reçu du fils : %s\n", buffer);
        }
        else if (bytesRead == 0)
        {
            // Toutes les données du tube ont été lues, sortie de la boucle
            break;
        }
        else if (bytesRead == -1)
        {
            printf("%d) Pas de message du fils.\n", k);
            // Les données ne sont pas encore disponibles, attendre un peu
            usleep(1000000); // Attendre 0.5 seconde
        }
        else
        {
            perror("Erreur de lecture");
            break;
        }
    }
    close(numPipe[0]);
}

int main()
{
    int fd1[2], fd2[2];
    pid_t pid1, pid2;
    srand(time(NULL));

    if (pipe(fd1) == -1)
    {
        perror("Erreur de la création du tube");
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
        close(fd1[0]);
        int messagesCount = 10; // Générer 10 messages
        int i = 0;
        int delay = 1;
        while (i < messagesCount)
        {
            char maChaine[] = "msg"; // Exemple de chaîne
            int longueur = strlen(maChaine);
            int numero = myRandom(1, 3);
            char message[100];
            char msg1[20] = "bonjour";
            char msg2[20] = "au revoir";
            char msg3[20] = "a demain";
            if (numero == 1)
            {
                snprintf(message, sizeof(message), "%s (message du fils 1 après %d secondes)", msg1, delay);
            }
            else if (numero == 2)
            {
                snprintf(message, sizeof(message), "%s (message du fils 1 après %d secondes)", msg2, delay);
            }
            else if (numero == 3)
            {
                snprintf(message, sizeof(message), "%s (message du fils 1 après %d secondes)", msg3, delay);
            }
            write(fd1[1], message, strlen(message) + 1);
            delay = myRandom(4, 6); // Générer un délai aléatoire entre 4 et 6 secondes
            sleep(delay);           // Attendre le délai généré aléatoirement
            i++;
        }
        close(fd1[1]);
    }
    else
    {

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
            close(fd1[0]);
            int messagesCount = 3; // Générer 10 messages
            int i = 0;
            int delay = 1;
            while (i < messagesCount)
            {
                char maChaine[] = "msg"; // Exemple de chaîne
                int longueur = strlen(maChaine);
                int numero = myRandom(1, 3);
                char message[100];
                char msg1[20] = "bonjour";
                char msg2[20] = "au revoir";
                char msg3[20] = "a demain";
                if (numero == 1)
                {
                    snprintf(message, sizeof(message), "%s (message du fils 2 après %d secondes)", msg1, delay);
                }
                else if (numero == 2)
                {
                    snprintf(message, sizeof(message), "%s (message du fils 2 après %d secondes)", msg2, delay);
                }
                else if (numero == 3)
                {
                    snprintf(message, sizeof(message), "%s (message du fils 2 après %d secondes)", msg3, delay);
                }
                write(fd1[1], message, strlen(message) + 1);
                delay = myRandom(4, 6); // Générer un délai aléatoire entre 4 et 6 secondes
                sleep(delay);           // Attendre le délai généré aléatoirement
                i++; 
            }
            close(fd1[1]);
        }
        else
        {
            fctParent(fd2);
            fctParent(fd1);
        }

        return 0;
    }
}

    // // test pipes bloquants
    // // ps -aux | grep "myprocess"
    // // echo   $(ps -aux | grep "pipeBlo" | awk '{ print $2 }')
    // // kill   $(ps -aux | grep "pipeBlo" | awk '{ print $2 }')

    // #include <stdio.h>
    // #include <stdlib.h>
    // #include <unistd.h>
    // #include <string.h>
    // #include <sys/types.h>
    // #include <sys/wait.h>
    // #include <fcntl.h>
    // #include <time.h>

    // int myRandom(int min, int max)
    // {
    //     int nombreAlea = rand() % (max - min + 1) + min;
    //     return nombreAlea;
    // }

    // int myFct(char msg[20], char buffer[150])
    // {
    //     printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg, buffer);
    // }
    // int main()
    // {
    //     int fd[2];
    //     pid_t pid;
    //     srand(time(NULL));

    //     if (pipe(fd) == -1)
    //     {
    //         perror("Erreur de la création du tube");
    //         exit(EXIT_FAILURE);
    //     }

    //     pid = fork();

    //     if (pid == -1)
    //     {
    //         perror("Erreur lors de la création du processus fils");
    //         exit(EXIT_FAILURE);
    //     }

    //     if (pid == 0)
    //     {
    //         close(fd[0]);
    //         int messagesCount = 10; // Générer 10 messages
    //         int i = 0;
    //         int delay = 1;
    //         while (i < messagesCount)
    //         {
    //             char maChaine[] = "msg"; // Exemple de chaîne
    //             int longueur = strlen(maChaine);
    //             int numero = myRandom(1, 3);
    //             char message[100];
    //             char msg1[20] = "bonjour";
    //             char msg2[20] = "au revoir";
    //             char msg3[20] = "a demain";
    //             if (numero == 1)
    //             {
    //                 snprintf(message, sizeof(message), "%s (message après %d secondes)", msg1, delay);
    //             }
    //             else if (numero == 2)
    //             {
    //                 snprintf(message, sizeof(message), "%s (message après %d secondes)", msg2, delay);
    //             }
    //             else if (numero == 3)
    //             {
    //                 snprintf(message, sizeof(message), "%s (message après %d secondes)", msg3, delay);
    //             }
    //             write(fd[1], message, strlen(message) + 1);
    //             delay = myRandom(4, 6); // Générer un délai aléatoire entre 4 et 6 secondes
    //             sleep(delay);           // Attendre le délai généré aléatoirement
    //             i++;
    //         }
    //         close(fd[1]);
    //     }
    //     else
    //     {

    //         // Code du processus parent
    //         close(fd[1]); // Fermer le descripteur d'écriture, car le parent lit seulement
    //         char buffer[150];
    //         int bytesRead;
    //         //  int status;
    //         //         waitpid(pid, &status, 0); // Attendre que le processus fils se termine
    //         fcntl(fd[0], F_SETFL, O_NONBLOCK);
    //         int k = 0;
    //         while (1)
    //         {
    //             k++;
    //             // Lecture non bloquante
    //             bytesRead = read(fd[0], buffer, sizeof(buffer));
    //             if (bytesRead > 0)
    //             {
    //                 char msg1[20] = "bonjour";
    //                 char msg2[20] = "au revoir";
    //                 char msg3[20] = "a demain";
    //                 k = 0;
    //                 if (strstr(buffer, msg1) != NULL)
    //                 {
    //                     // printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg1, buffer);
    //                     myFct(msg1, buffer);
    //                 }
    //                 // else
    //                 // {
    //                 //     printf("Le mot '%s' et '%s' n'ont pas été trouvé dans la phrase : '%s'\n", msg2, msg3, buffer);
    //                 // }
    //                 else if (strstr(buffer, msg2) != NULL)
    //                 {
    //                     // printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg2, buffer);
    //                     myFct(msg2, buffer);
    //                 }
    //                 // else
    //                 // {
    //                 //     printf("Le mot '%s' et '%s' n'ont pas été trouvé dans la phrase : '%s'\n", msg1, msg3, buffer);
    //                 // }
    //                 else if (strstr(buffer, msg3) != NULL)
    //                 {
    //                     // printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg3, buffer);
    //                     myFct(msg3, buffer);
    //                 }
    //                 // else
    //                 // {
    //                 //     printf("Le mot '%s' et '%s' n'ont pas été trouvé dans la phrase : '%s'\n", msg1, msg2, buffer);
    //                 // }
    //                 // printf("Message reçu du fils : %s\n", buffer);
    //             }
    //             else if (bytesRead == 0)
    //             {
    //                 // Toutes les données du tube ont été lues, sortie de la boucle
    //                 break;
    //             }
    //             else if (bytesRead == -1)
    //             {
    //                 printf("%d) Pas de message du fils.\n", k);
    //                 // Les données ne sont pas encore disponibles, attendre un peu
    //                 usleep(1000000); // Attendre 0.5 seconde
    //             }
    //             else
    //             {
    //                 perror("Erreur de lecture");
    //                 break;
    //             }
    //         }
    //         close(fd[0]);
    //     }

    //     return 0;
    // }
