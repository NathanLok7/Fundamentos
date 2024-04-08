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
    for (i = 0; i < n; i++)
        numerator *= x;
    if (n % 2 == 0)
        return (-numerator / n);
    else
        return numerator / n;
}

void child_proc(int proc_num, int read_pipe) {
    close(read_pipe); // Cerrar extremo de lectura no utilizado en el proceso hijo
    
    double sum = 0;
    for (int i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS) {
        sum += get_member(i + 1, x);
    }
    
    // Escribir la suma parcial en el pipe
    write(STDOUT_FILENO, &sum, sizeof(double));
    
    exit(0);
}

int main() {
    int i;
    int pipes[NPROCS][2]; // Arreglo de pipes
    
    // Crear pipes para cada proceso hijo
    for (i = 0; i < NPROCS; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    
    // Crear procesos hijos
    for (i = 0; i < NPROCS; i++) {
        pid_t pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
            child_proc(i, pipes[i][0]); // Llamar a la función del hijo con el extremo de lectura del pipe
        } else {
            // Proceso padre
            close(pipes[i][0]); // Cerrar extremo de lectura no utilizado en el padre
        }
    }
    
    // Proceso padre lee las sumas parciales de los procesos hijos
    double total_sum = 0;
    double partial_sum;
    
    for (i = 0; i < NPROCS; i++) {
        // Leer la suma parcial desde el pipe
        read(pipes[i][0], &partial_sum, sizeof(double));
        total_sum += partial_sum;
        
        close(pipes[i][0]); // Cerrar descriptor de lectura después de leer
    }
    
    // Imprimir resultados
    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n", SERIES_MEMBER_COUNT);
    printf("El valor del argumento x es %f\n", x);
    printf("El resultado es %10.8f\n", total_sum);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n", x, log(1 + x));
    
    return 0;
}
