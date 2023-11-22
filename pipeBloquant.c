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

// cette function est ex
void vivre_ma_vie_de_fils(int nr_fils, int pipe[2])
{

    // srand(time(NULL)+nr_fils);
    close(pipe[0]);
    int messagesCount;
    char msg[100];
    snprintf(msg, sizeof(msg), "ouverture du pipe du fils nr %d je démarre\n", nr_fils);  
    write(pipe[1], msg, strlen(msg) + 1);
    if (nr_fils == 1)
    {
        messagesCount = 3;
    }
    else
    {
        messagesCount = 5;
    }
    int i = 0;
    int delay = 1;
    // while (i < messagesCount)
    // {
    //     char maChaine[] = "msg"; // Exemple de chaîne
    //     int longueur = strlen(maChaine);
    //     int numero = myRandom(1, 3);
    //     char message[100];
    //     char msg1[20] = "bonjour";
    //     char msg2[20] = "au revoir";
    //     char msg3[20] = "a demain";
    //     if (numero == 1)
    //     {
    //         snprintf(message, sizeof(message), "%s (message %d du fils 2 après %d secondes)", msg1, i, delay);
    //     }
    //     else if (numero == 2)
    //     {
    //         snprintf(message, sizeof(message), "%s (message %d du fils 2 après %d secondes)", msg2, i, delay);
    //     }
    //     else if (numero == 3)
    //     {
    //         snprintf(message, sizeof(message), "%s (message %d du fils 2 après %d secondes)", msg3, i, delay);
    //     }
    //     write(pipe[1], message, strlen(message) + 1);
    //     delay = myRandom(2, 3); // Générer un délai aléatoire entre 4 et 6 secondes
    //     sleep(delay);           // Attendre le délai généré aléatoirement
    //     i++;
    // }
    close(pipe[1]);
}

// cette fonction regarde dans le pipe passé en param s'il y a un message à traiter
// si c'est le cas, on traite le message et on sort
// sinon, on sort directement
// on revoie comme résultat
//      1 si le pipe est encore ouvert (càd pas encore fermé par le fils)
//      0 si le pipe a été fermé par le fils
// NB : nb_bytes_read = read(pipe, buffer, sizeof(buffer)
//    si nb_bytes_read =
//          > 0 : il y a des char dans le pipe
//          = 0 : le pipe a été clôturé
//          -1  : pas de char dans le pipe pour le moment
int traiterContenuActuelDuPipe(int nr_fils, int numPipe[])
{
    char buffer[150];

    fcntl(numPipe[0], F_SETFL, O_NONBLOCK);

    int bytesRead = read(numPipe[0], buffer, sizeof(buffer));
    if (bytesRead > 0)
    {
        char msg1[20] = "bonjour";
        char msg2[20] = "au revoir";
        char msg3[20] = "a demain";
        printf(buffer);
        if (strstr(buffer, msg1) != NULL)
        {
            myFct(msg1, buffer); // Assurez-vous que myFct est définie correctement
        }
        else if (strstr(buffer, msg2) != NULL)
        {
            myFct(msg2, buffer);
        }
        else if (strstr(buffer, msg3) != NULL)
        {
            myFct(msg3, buffer);
        }
        else if (strstr(buffer, "je démarre") != NULL)
        {
            printf("|-----|-----|-----|-----|-----|\n");
        }
        return 1; // le pipe est encore ouvert
    }
    else if (bytesRead == 0)
    {
        printf("Le pipe du fils nr %d a été clôturé par le fils\n", nr_fils);
        return 0; // le pipe a été clôturé
    }
    else if (bytesRead == -1)
    {
        // printf("Pas de données dans le pipe du fils %d pour le moment\n", nr_fils);
        return 1; // le pipe est encore ouvert
    }
}

void gerer_mes_fils(int pipe1[2], int pipe2[2])
{
    close(pipe1[1]); // Fermer le descripteur d'écriture, car le parent lit seulement
    close(pipe2[1]); // Fermer le descripteur d'écriture, car le parent lit seulement

    int k = 0;
    while (1)
    {
        k++;
        printf("%d)\n", k);
        int statusPipe1 = traiterContenuActuelDuPipe(1, pipe1);
        int statusPipe2 = traiterContenuActuelDuPipe(2, pipe2);

        if (statusPipe1 == 0 && statusPipe2 == 0)
        {
            printf("Les deux pipes ont été fermés par les fils.\n");
            break; // Sortir de la boucle infinie
        }
        else
        {
            char buffer[20];
            int bytesRead = read(pipe1[0], buffer, sizeof(buffer));
            if (bytesRead > 0)
            {
                printf("Message reçu depuis le fils 1 : %s\n", buffer);
            }

            bytesRead = read(pipe2[0], buffer, sizeof(buffer));
            if (bytesRead > 0)
            {
                printf("Message reçu depuis le fils 2 : %s\n", buffer);
            }
        }
        // printf("statusPipe1 : %d; statusPipe2 : %d\n", statusPipe1, statusPipe2);
        usleep(1000000); // attendre x microsecond
    }
    close(pipe1[0]); // Fermer le descripteur de lecture du fils 1
    close(pipe2[0]); // Fermer le descripteur de lecture du fils 2
}

int main()
{
    int pipe1[2], pipe2[2];
    // int pipe[20][2];
    pid_t pid1, pid2;

    if (pipe(pipe1) == -1)
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
        // code du fils 1
        vivre_ma_vie_de_fils(1, pipe1);
    }
    else
    {
        if (pipe(pipe2) == -1)
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
            vivre_ma_vie_de_fils(2, pipe2);
        }
        else
        {
            // Code du processus parent
            gerer_mes_fils(pipe1, pipe2);
            return 0;
        }
    }
}
