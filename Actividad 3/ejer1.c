
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p;
    p = fork();
    
    if(p < 0) {
        perror("Error al crear el proceso hijo. \n");
    } 
    else if(p == 0) {
        for(int i = 0; i < 10; i++) {
            printf("Soy el hijo %d\n", getpid());
            sleep(1); 
        }
    } 
    else {
        for(int i = 0; i < 10; i++) {
            printf("Soy el padre %d\n", getpid());
            sleep(1); 
        }
        wait(NULL);
        printf("Mi proceso hijo ya ha terminado \n");
    }
    return 0;
}
