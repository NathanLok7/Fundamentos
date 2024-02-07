
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("Error al crear proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Si existe el padre se ejecuta el codigo del hijo
        printf("Proceso hijo iniciado. PID: %d\n", getpid());
        sleep(5);  // El proceso hijo espera 5 segundos
        printf("Proceso hijo finalizado.\n");
    } else {
        // Código del proceso padre
        printf("Proceso padre iniciado. PID: %d\n", getpid());
        sleep(10); // El proceso padre espera 10 segundos
        printf("Proceso padre finalizado.\n");
    }

    return 0;
}