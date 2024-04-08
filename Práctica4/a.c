#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>

#define NPROCS 4
#define SERIES_MEMBER_COUNT 200000

double x = 1.0;

double get_member(int n, double x) {
    int i;
    double numerator = 1;
    for(i = 0; i < n; i++)
        numerator = numerator * x;
    if (n % 2 == 0)
        return ( - numerator / n );
    else
        return numerator / n;
}

void proc(int proc_num, int write_pipe) {
    int i;
    double sum = 0;
    for(i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS)
        sum += get_member(i + 1, x);
    write(write_pipe, &sum, sizeof(double));
    close(write_pipe);
    exit(0);
}

void master_proc(int* read_pipes) {
    int i;
    double sum = 0;
    for (i = 0; i < NPROCS; i++) {
        double partial_sum;
        read(read_pipes[i], &partial_sum, sizeof(double));
        sum += partial_sum;
        close(read_pipes[i]);
    }
    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
    printf("El valor del argumento x es %f\n", x);
    printf("El resultado es %10.8f\n", sum);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1+x));
    exit(0);
}

int main() {
    int pipes[NPROCS][2];
    pid_t pids[NPROCS + 1];
    int i;

    for (i = 0; i < NPROCS; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < NPROCS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            close(pipes[i][0]); // Close read end of the pipe
            proc(i, pipes[i][1]); // Call child process
        }
    }

    for (i = 0; i < NPROCS; i++) {
        close(pipes[i][1]); // Close write end of the pipe in parent
    }

    pids[NPROCS] = fork();
    if (pids[NPROCS] == 0) {
        master_proc((int*)pipes); // Call master process
    }

    // Wait for all child processes to finish
    for (i = 0; i < NPROCS + 1; i++) {
        wait(NULL);
    }

    return 0;
}
