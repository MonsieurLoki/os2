#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// int main()
// {
//     srand(time(NULL));

//     int min = 25;
//     int max = 45;
//     int nbrAleat = rand() % (max - min + 1) + min; // +1 car sinon le nombre est entre 25 et 44

//     printf("Nombre aléatoire entre %d et %d : %d\n", min, max, nbrAleat);
//     return 0;
// }

int myRandom(int min, int max)
{
    int nombreAlea = rand() % (max - min + 1) + min;

    return nombreAlea;
}

int main()
{
        srand(time(NULL));

    int cnt = 30;
    int i = 0;
    while (i < cnt)
    {
        i++;
        int nombre = myRandom(25, 27);
        printf("%d) %d\n", i, nombre);
    }
    return 0;
}