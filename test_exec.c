#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

    /* Lista de argumentos para o processo a ser carregado */
    /* [0]: nome do programa, [1]: argumentos */
    char *const ps_argv[] = {"echo", "alo mundo 3 e 4", 0};

    /* Chamadas de sistema do tipo exec */
    execl("/bin/echo", "echo", "alo mundo", 0);

    /* assumindo que echo esta em /bin */
    execlp("echo", "echo", "alo mundo 2", 0);

    /* assumindo que echo esta em /bin */
    execv("/bin/echo", ps_argv);

    /* assumindo que /bin esta em PATH */
    execvp("echo", ps_argv);

    exit(0);
}
