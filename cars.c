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

#define NB_VOITURE 20
// int positionCar1;
// int positionCar2;
int positionCar[NB_VOITURE];

const int nombreTirets = 70;
void dessiner(int position)
{
    char buffer[nombreTirets + 1];
    for (int i = 0; i < nombreTirets; i++)
    {
        buffer[i] = '-';
    }
    buffer[nombreTirets] = '\0';
    buffer[nombreTirets - 1] = '|';
    int positionEtoile = (nombreTirets - 1) * position / 100;
    buffer[positionEtoile] = '*';
    printf("%s\n", buffer);
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
void vivre_ma_vie_de_voiture(int nr_car, int pipe[2])
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
        snprintf(msg, sizeof(msg), "je suis la voiture numéro %d et je suis à la position **, sec: %d, duree: %d |%d\n", nr_car, sec, dureeCourse, position);
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

    // faire en sorte que le pipe ne soit pas bloquant
    fcntl(numPipe[0], F_SETFL, O_NONBLOCK);

    int bytesRead = read(numPipe[0], buffer, sizeof(buffer));
    if (bytesRead > 0)
    {
        // printf(buffer);
        // if (strstr(buffer, "bonjour") != NULL)
        // {
        //     traiterMessage("bonjour", buffer); // Assurez-vous que traiterMessage est définie correctement
        // }
        // else if (strstr(buffer, "au revoir") != NULL)
        // {
        //     traiterMessage("au revoir", buffer);
        // }
        // else if (strstr(buffer, "a demain") != NULL)
        // {
        //     traiterMessage("a demain", buffer);
        // }
        if (strstr(buffer, "je démarre") != NULL)
        {
            positionCar[nr_car] = 0;
        }
        else if (strstr(buffer, "arriver") != NULL)
        {

            positionCar[nr_car] = 100;
        }
        else if (strstr(buffer, "position") != NULL)
        {
            positionCar[nr_car] = getValue(buffer);
        }
        else
        {
            printf("Message inconnu : %s\n", buffer);
        }
        return 1; // le pipe est encore ouvert
    }

    else if (bytesRead == 0)
    {
        // printf("Le pipe du fils nr %d a été clôturé par le fils\n", nr_car);
        return 0; // le pipe a été clôturé
    }
    else if (bytesRead == -1)
    {
        // printf("Pas de données dans le pipe du fils %d pour le moment\n", nr_car);
        return 1; // le pipe est encore ouvert
    }
}

// void gererVoitures(int pipe1[2], int pipe2[2])
void gererVoitures(int pipe[][2])
{
    // close(pipe1[1]); // Fermer le descripteur d'écriture, car le parent lit seulement
    // close(pipe2[1]); // Fermer le descripteur d'écriture, car le parent lit seulement

    for (int i = 0; i < NB_VOITURE; i++)
    {
        close(pipe[i][1]);
    }
    int k = 0;
    while (1)
    {
        printf("\n");
        k++;
        int statusPipe[NB_VOITURE];
        for (int i = 0; i < NB_VOITURE; i++)
        {
            statusPipe[i] = traiterContenuActuelDuPipe(i, pipe[i]);

            printf("%3d) voiture %2d: %3d ", k, i, positionCar[i]);
            // dessiner(positionCar1);
            // dessiner(positionCar2);

            dessiner(positionCar[i]);
            // int statusPipe1 = traiterContenuActuelDuPipe(1, pipe1);
            // int statusPipe2 = traiterContenuActuelDuPipe(2, pipe2);

            // else
            // {
            //     char buffer[20];
            //     int bytesRead = read(pipe[i][0], buffer, sizeof(buffer));
            //     if (bytesRead > 0)
            //     {
            //         printf("Message reçu depuis le fils %d : %s\n", i, buffer);
            //     }

            //     // bytesRead = read(pipe[0], buffer, sizeof(buffer));
            //     // if (bytesRead > 0)
            //     // {
            //     //     printf("Message reçu depuis le fils 2 : %s\n", buffer);
            //     // }
            // }
            // printf("statusPipe1 : %d; statusPipe2 : %d\n", statusPipe1, statusPipe2);
        }

        // if (statusPipe1 == 0 && statusPipe2 == 0)

        int auMoinsUnPipeEncoreOuvert = 0;
        for (int k = 0; k < NB_VOITURE; k++)
        {
            if (statusPipe[k] != 0)
            {
                // au moins un des pipes est encore ouvert !
                auMoinsUnPipeEncoreOuvert = 1;
            }
        }

        if (auMoinsUnPipeEncoreOuvert == 0)
        {
            break; // Sortir de la boucle infinie
        }

        usleep(200 * 1000); // attendre x microsecond
    }

    // printf("Tous les pipes ont été fermés par les fils.\n");
    // close(pipe1[0]); // Fermer le descripteur de lecture du fils 1
    // close(pipe2[0]); // Fermer le descripteur de lecture du fils 2
    for (int k = 0; k < NB_VOITURE; k++)
    {
        close(pipe[k][0]); // Fermer le descripteur de lecture du fils
    }
}

int lancerVoiture(int nr_voiture, int Mypipe[])
{
    if (pipe(Mypipe) == -1)
    {
        perror("Erreur de la création du tube");
        exit(EXIT_FAILURE);
    }
    int pid;
    pid = fork();

    if (pid == -1)
    {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        vivre_ma_vie_de_voiture(nr_voiture, Mypipe);
        exit(0);
    }
    else
    {
        return pid;
    }
}

int main()
{
    // int pipe1[2], pipe2[2];
    int pipe[NB_VOITURE][2];
    // pid_t pid1, pid2;
    pid_t pid[NB_VOITURE];
    // pid1 = lancerVoiture(1, pipe1);
    // pid2 = lancerVoiture(2, pipe2);
    for (int i = 0; i < NB_VOITURE; i++)
    {
        pid[i] = lancerVoiture(i, pipe[i]);
    }
    gererVoitures(pipe);
}
