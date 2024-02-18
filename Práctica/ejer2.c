#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#define NUM_THREADS 4 // Número de hilos a utilizar
#define N 100000000 // Número de intervalos para la aproximación

double sum = 0.0; // Variable global para almacenar la suma de las áreas

// Función para calcular f(x) = sqrt(1 - y^2)
double f(double x) {
    return sqrt(1 - x * x);
}

// Función que ejecuta cada hilo
void *thread_func(void *arg) {
    long thread_id = (long)arg;
    double partial_sum = 0.0;
    double base = 1.0 / N;
    long chunk_size = N / NUM_THREADS;
    long start = thread_id * chunk_size;
    long end = (thread_id + 1) * chunk_size;

    // Calcular área utilizando la regla del trapecio
    for (long i = start; i < end; i++) {
        double x0 = i * base;
        double x1 = (i + 1) * base;
        double height = (f(x0) + f(x1)) / 2.0;
        partial_sum += height * base;
    }

    // Agregar la suma parcial a la suma global
    sum += partial_sum;

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    struct timeval start, end;
    double tiempo;

    gettimeofday(&start, NULL); // Tiempo inicial

    // Crear hilos
    for (long t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, thread_func, (void *)t);
        if (rc) {
            printf("Error: No se pudo crear el hilo %ld\n", t);
            exit(-1);
        }
    }

    // Esperar a que todos los hilos terminen
    for (long t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Multiplicar la suma por 4 para obtener PI
    double pi = 4 * sum;

    gettimeofday(&end, NULL); // Tiempo final

    tiempo = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000; // Cálculo del tiempo transcurrido

    // Imprimir resultado
    printf("Valor aproximado de PI: %lf\n", pi);
    printf("Tiempo transcurrido: %.6f segundos\n", tiempo);

    return 0;
}
