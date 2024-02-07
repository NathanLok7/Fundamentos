#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p;
    p = fork();

    if(p < 0) {
        perror("Error al crear el proceso hijo.\n");
    } else if (p == 0) {
        printf("Soy el hijo\n");
        sleep(20);  
    } 
    else{
        printf("Soy el padre\n");
        sleep(1);   
        system("ps --user $USER f");

        wait(NULL); 
        printf("Mi proceso hijo ya ha terminado\n");
    }
    
    return 0;
}
