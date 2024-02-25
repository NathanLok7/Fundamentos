#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int p;
    p = fork();
    
    for (int i = 0; i < 10; i++)
    {
        if(p == 0){
            printf("Soy el hijo\n");
            sleep(1);
        }
        if(p > 0){
            printf("Soy el padre\n");
            sleep(1);
        }
    }
    if(p > 0){
        wait(NULL);
        printf("voy a esperar\n");
    }

    return 0;
}

