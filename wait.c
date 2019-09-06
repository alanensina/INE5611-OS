#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    char *message;
    int n;
    int exit_code;

    printf("Programa exemplo fork iniciou\n");
    pid = fork();
    switch(pid) 
    {
    case -1:
        exit(1);
    case 0:
        message = "Este é o processo filho\n";
        n = 5;
        exit_code = 37;
        break;
    default:
        message = "Este é o processo pai\n";
        n = 3;
        exit_code = 0;
        break;
    }

    for(; n > 0; n--) {
        printf("%s",message);
        sleep(1);
    }

/*  A seguir o programa espera pelo processo filho para terminar.  */

    if(pid) {       // se diferente de zero - ou seja, processo pai
        int stat_val;
        pid_t child_pid;

        child_pid = wait(&stat_val);

        printf("Filho terminou: PID = %d\n", child_pid);
        if(WIFEXITED(stat_val))
            printf("Filho saiu com código %d\n", WEXITSTATUS(stat_val));
                
        else
            printf("Filho terminou de forma anormal\n");
    }
    exit (exit_code);
}
