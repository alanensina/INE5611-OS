#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    char *message;
    int n;

    printf("Programa exemplo fork iniciou\n");
    pid = fork();
    switch(pid) 
    {
    case -1:
        perror("fork falhou");
        exit(1);
    case 0:
        message = "Este é o processo filho\n";
        n = 3;
        break;
    default:
        message = "Este é o processo pai\n";
        n = 5;
        break;
    }

    for(; n > 0; n--) {
        printf("%s",message);
        sleep(1);
    }
    exit(0);
}
