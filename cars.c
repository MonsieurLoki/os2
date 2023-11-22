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

int positionCar1;
int positionCar2;

const int nombreTirets = 70;
void dessiner(int position)
{
    char buffer[nombreTirets + 1];
    for (int i = 0; i < nombreTirets; i++)
    {
        buffer[i] = '-';
    }
    buffer[sizeof(buffer) - 1] = '\0';
    int positionEtoile = nombreTirets * position / 100;
    buffer[positionEtoile] = '*';
    printf("%s\n\n", buffer);
}

// prend la valeur a la fin de la chaine de caractère après la |
int getValue(char *chaine)
{
    // Chercher la position du caractère '|'
    char *separator = strchr(chaine, '|');

    if (separator != NULL)
    {
        // Utiliser strtok pour diviser la chaîne à partir de '|'
        char *token = strtok(separator, "|");

        // Utiliser atoi pour convertir le token suivant '|' en entier
        if (token != NULL)
        {
            return atoi(token);
        }
    }

    return -1; // Valeur par défaut si le symbole '|' n'est pas trouvé ou s'il n'y a pas de valeur après '|'
}

int myRandom(int min, int max)
{
    int nombreAlea = rand() % (max - min + 1) + min;
    return nombreAlea;
}

int traiterMessage(char msg[20], char buffer[150])
{
    printf("Le mot '%s' a été trouvé dans la phrase : '%s'\n", msg, buffer);
}

// cette function est ex
void vivre_ma_vie_de_fils(int nr_car, int pipe[2])
{
    srand(time(NULL) + nr_car);
    close(pipe[0]);
    int messagesCount;
    char msg[1000];
    snprintf(msg, sizeof(msg), "je suis la voiture numéro %d et je démarre\n", nr_car);
    write(pipe[1], msg, strlen(msg) + 1);
    int dureeCourse = myRandom(7, 12);
    int sec = 0;
    int position;
    while (sec <= dureeCourse)
    {
        position = ((100 * sec) / dureeCourse);
        snprintf(msg, sizeof(msg), "je suis la voiture numéro %d et je suis à la position, sec: %d, duree: %d |%d\n", nr_car, sec, dureeCourse, position);
        write(pipe[1], msg, strlen(msg) + 1);
        sleep(1);
        sec++;
    }
    snprintf(msg, sizeof(msg), "je suis la voiture numéro %d et je viens d'arriver\n", nr_car);
    write(pipe[1], msg, strlen(msg) + 1);
    int i = 0;
    int delay = 1;
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
int traiterContenuActuelDuPipe(int nr_car, int numPipe[])
{
    char buffer[150];

    fcntl(numPipe[0], F_SETFL, O_NONBLOCK);

    int bytesRead = read(numPipe[0], buffer, sizeof(buffer));
    if (bytesRead > 0)
    {
        // printf(buffer);
        if (strstr(buffer, "bonjour") != NULL)
        {
            traiterMessage("bonjour", buffer); // Assurez-vous que traiterMessage est définie correctement
        }
        else if (strstr(buffer, "au revoir") != NULL)
        {
            traiterMessage("au revoir", buffer);
        }
        else if (strstr(buffer, "a demain") != NULL)
        {
            traiterMessage("a demain", buffer);
        }
        else if (strstr(buffer, "je démarre") != NULL)
        {
            if (nr_car == 1)
            {
                positionCar1 = 0;
            }
            if (nr_car == 2)
            {
                positionCar2 = 0;
            }
        }
        else if (strstr(buffer, "arriver") != NULL)
        {
            if (nr_car == 1)
            {
                positionCar1 = 100;
            }
            if (nr_car == 2)
            {
                positionCar2 = 100;
            }
        }
        else if (strstr(buffer, "position") != NULL)
        {
            if (nr_car == 1)
            {
                positionCar1 = getValue(buffer);
            }
            if (nr_car == 2)
            {
                positionCar2 = getValue(buffer);
            }
        }
        else
        {
            printf("Message inconnu : %s\n", buffer);
        }
        return 1; // le pipe est encore ouvert
    }
    else if (bytesRead == 0)
    {
        printf("Le pipe du fils nr %d a été clôturé par le fils\n", nr_car);
        return 0; // le pipe a été clôturé
    }
    else if (bytesRead == -1)
    {
        // printf("Pas de données dans le pipe du fils %d pour le moment\n", nr_car);
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
        printf("%d) voiture 1: %d, voiture 2: %d\n", k, positionCar1, positionCar2);
        dessiner(positionCar1);
        dessiner(positionCar2);
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
