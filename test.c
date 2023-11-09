#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//fork père et fil plus leur pid

// int main (){
//     int fd[2];
//     __pid_t pid;
//     if (pipe (fd)<0){
//         printf("erreur d'ouvertur du pipe");
//         return 1;
//     }
//     pid = fork();
//     if (pid == -1) printf("erreur de création d'un processus fils");
//     if (pid == 0) printf("c'est le fil, pid : %d\n", getpid());
//     else{ printf("c'est le père, pid : %d\n", getpid());}
// }

//fork père et fils plus envoie d'un message du fils au père avec le pipe
// #define N 50
// int main (){

//     int stdout_fd = fileno(stdout);

//     int fd[2];

//     char message[N];
//     int n;

//     __pid_t pid;
    
//     if (pipe (fd)<0){
//         printf("erreur d'ouvertur du pipe");
//         return 1;
//     }
//     pid = fork();
//     if (pid == -1) printf("erreur de création d'un processus fils");
//     else {
//         if( pid == 0){ // proc fils
        
//             close( fd[1] );

//             n = read( fd[0], message, N );

//             write(stdout_fd, message, n);

//         }else { // proc père
//             close( fd[0] );
//             write( fd[1], "Bonjour mon fils", strlen("Bonjour mon fils") );
//             close( fd[1] );
//         }
//     }
// }

//même chose mais avec deux processus
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define N 50

int main() {
    int fd1[2], fd2[2];
    char message[N];
    int n;
    __pid_t pid;

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("Erreur d'ouverture du pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        printf("Erreur de création d'un processus fils");
    } else if (pid == 0) { // Premier processus fils
        close(fd1[1]);
        n = read(fd1[0], message, N);
        write(STDOUT_FILENO, message, n);
    } else { // Processus père
        __pid_t pid2 = fork(); // Création du deuxième processus fils
        if (pid2 == -1) {
            printf("Erreur de création du deuxième processus fils");
        } else if (pid2 == 0) { // Deuxième processus fils
            close(fd2[1]);
            n = read(fd2[0], message, N);
            write(STDOUT_FILENO, message, n);
        } else { // Processus père
            close(fd1[0]);
            write(fd1[1], "Bonjour mon premier fils\n", strlen("Bonjour mon premier fils\n"));
            close(fd1[1]);

            close(fd2[0]);
            write(fd2[1], "Bonjour mon deuxième fils\n", strlen("Bonjour mon deuxième fils\n"));
            close(fd2[1]);
        }
    }

    return 0;
}
