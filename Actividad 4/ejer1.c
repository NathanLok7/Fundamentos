
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sig_handler(int signo) {
    if (signo == SIGTERM)
        printf("Proceso hijo recibió SIGTERM\n");
}

int main() {

    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("Error al crear proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Si existe el padre se ejecuta el codigo del hijo
        printf("Proceso hijo iniciado. PID: %d\n", getpid());
        signal(SIGTERM, sig_handler); // Manejar la señal SIGTERM
        pause();
        printf("Proceso hijo finalizado.\n");
    } else {
        // Código del proceso padre
        printf("Proceso padre iniciado. PID: %d\n", getpid());
        sleep(5); // Esperar 5 segundos
        printf("Matando al proceso hijo. PID: %d\n", pid);
        kill(pid, SIGTERM); // Enviar señal SIGTERM al proceso hijo
        wait(NULL); // Esperar a que el proceso hijo termine
        printf("Proceso padre finalizado.\n");
    }

    return 0;
}

