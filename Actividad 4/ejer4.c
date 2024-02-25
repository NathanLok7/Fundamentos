#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void crear_3Nietos() {
    for(int i = 0; i < 3; i++) {
        pid_t nieto = fork();

        if(nieto < 0) {
            perror("Error al crear el proceso nieto.\n");
            return;
        } 
        else if (nieto == 0) {
            printf("Soy el proceso nieto con PID: %d, mi padre es: %d\n", getpid(), getppid());
            sleep(20);
            exit(EXIT_SUCCESS);
        }
    }
}

int main() {
    pid_t hijo1, hijo2;

    hijo1 = fork();

    if(hijo1 < 0) {
        perror("Error al crear el primer proceso hijo.\n");
        exit(EXIT_FAILURE);
    } 
    else if(hijo1 == 0) { //hijo
        printf("Soy el primer hijo con PID: %d, mi padre es: %d\n", getpid(), getppid());
        crear_3Nietos();
        sleep(20);
        exit(EXIT_SUCCESS);
    } 
    else{
        hijo2 = fork();

        if(hijo2 < 0) {
            perror("Error al crear el segundo proceso hijo.\n");
            exit(EXIT_FAILURE);
        } 
        else if(hijo2 == 0) {
            printf("Soy el segundo hijo con PID: %d, mi padre es: %d\n", getpid(), getppid());
            crear_3Nietos();
            sleep(20);
            exit(EXIT_SUCCESS);
        } 
        else{
            printf("Soy el padre con PID: %d\n", getpid());
            sleep(5);
            
            wait(NULL);

            printf("Mi proceso hijo ya ha terminado\n");
        }
    }
    return 0;
}


