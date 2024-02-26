#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_THREADS 4

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <start> <end>\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // Crear segmento de memoria compartida
    key_t key = ftok("/tmp", 'a');
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    int* total_primes = (int*)shmat(shmid, NULL, 0);
    *total_primes = 0;

    // Crear procesos hijos
    for (int i = 0; i < MAX_THREADS; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // Código del hijo
            int count = 0;
            for (int num = start + i; num <= end; num += MAX_THREADS) {
                if (is_prime(num)) {
                    count++;
                }
            }
            *total_primes += count;
            exit(0);
        } else if (pid < 0) {
            // Error al crear el proceso hijo
            fprintf(stderr, "Error creating child process\n");
            return 1;
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < MAX_THREADS; ++i) {
        wait(NULL);
    }

    gettimeofday(&end_time, NULL);
    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0; // Convertir a milisegundos
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    printf("Total primes between %d and %d: %d\n", start, end, *total_primes);
    printf("Execution time: %.2f milliseconds\n", execution_time);

    // Liberar la memoria compartida
    shmdt(total_primes);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
