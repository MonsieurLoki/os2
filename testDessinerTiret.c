#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main()
{
    dessiner(5);
    dessiner(10);
}