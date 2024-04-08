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
    for (i = 0; i < n; i++) {
        numerator = numerator * x;
    }
    if (n % 2 == 0) {
        return (-numerator / n);
    } else {
        return numerator / n;
    }
}

void slave_process(int proc_num, int read_fd) {
    double partial_sum = 0;
    for (int i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS) {
        partial_sum += get_member(i + 1, x);
    }
    // Write the partial sum to the pipe
    write(read_fd, &partial_sum, sizeof(double));
    exit(0);
}

int main() {
    int pipes[NPROCS][2]; // Array of pipes for communication
    double total_sum = 0;
    struct timeval start_time, stop_time;
    long long elapsed_time;

    // Start measuring time
    gettimeofday(&start_time, NULL);

    // Create pipes
    for (int i = 0; i < NPROCS; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }

        pid_t pid = fork();

        if (pid == 0) {
            // Child process (slave)
            close(pipes[i][0]); // Close read end of the pipe
            slave_process(i, pipes[i][1]); // Execute slave process
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        // Parent process continues to create more forks
        close(pipes[i][1]); // Close write end of the pipe
    }

    // Read partial sums from each child process
    for (int i = 0; i < NPROCS; i++) {
        double partial_sum;
        read(pipes[i][0], &partial_sum, sizeof(double));
        total_sum += partial_sum;
        close(pipes[i][0]); // Close read end of the pipe
    }

    // Stop measuring time
    gettimeofday(&stop_time, NULL);
    elapsed_time = (stop_time.tv_sec - start_time.tv_sec);

    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
    printf("El valor del argumento x es %f\n", x);
    printf("El resultado es %10.8f\n", total_sum);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1 + x));
    printf("Tiempo transcurrido: %lld segundos\n", elapsed_time);

    return 0;
}
