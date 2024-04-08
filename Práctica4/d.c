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

void slave_proc(int proc_num, int write_fd) {
    int i;
    double sum = 0.0;
    for (i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS) {
        sum += get_member(i + 1, x);
    }
    write(write_fd, &sum, sizeof(double));
    close(write_fd);
    exit(0);
}

void master_proc(int read_fds[]) {
    int i;
    double total_sum = 0.0;
    for (i = 0; i < NPROCS; i++) {
        double partial_sum;
        read(read_fds[i], &partial_sum, sizeof(double));
        total_sum += partial_sum;
        close(read_fds[i]);
    }
    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
    printf("El valor del argumento x es %f\n", x);
    printf("El resultado es %10.8f\n", total_sum);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1 + x));
}

int main() {
    struct timeval start_time, stop_time;
    long long elapsed_time;

    int pipe_fds[NPROCS][2];
    pid_t pid;
    int i;

    // Crear pipes para la comunicación entre procesos
    for (i = 0; i < NPROCS; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("Error al crear el pipe");
            exit(EXIT_FAILURE);
        }
    }

    gettimeofday(&start_time, NULL); // Tiempo inicial

    // Generar procesos hijos (esclavos)
    for (i = 0; i < NPROCS; i++) {
        pid = fork();
        if (pid == 0) {
            // Proceso esclavo
            close(pipe_fds[i][0]);  // Cerrar el extremo de lectura del pipe en el esclavo
            slave_proc(i, pipe_fds[i][1]);
        }
    }

    // Proceso padre (maestro)
    int read_fds[NPROCS];
    for (i = 0; i < NPROCS; i++) {
        close(pipe_fds[i][1]);  // Cerrar el extremo de escritura del pipe en el maestro
        read_fds[i] = pipe_fds[i][0];  // Mantener el extremo de lectura del pipe en el maestro
    }

    // Esperar a que todos los procesos hijos (esclavos) terminen
    for (i = 0; i < NPROCS; i++) {
        wait(NULL);
    }

    gettimeofday(&stop_time, NULL); // Tiempo final

    // Proceso maestro recoge los resultados de los esclavos
    master_proc(read_fds);

    // Calcular el tiempo transcurrido en segundos
    elapsed_time = (stop_time.tv_sec - start_time.tv_sec);

    printf("Tiempo = %lld segundos\n", elapsed_time);

    return 0;
}
