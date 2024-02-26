#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_THREADS 4

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    return 1;
}

void* count_primes(void* arg) {
    int* range = (int*)arg;
    int start = range[0];
    int end = range[1];
    int count = 0;

    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
            count++;
        }
    }

    int* result = malloc(sizeof(int));
    *result = count;
    return result;
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

    int total_primes = 0;
    pthread_t threads[MAX_THREADS];
    int thread_args[MAX_THREADS][2];

    int range_size = (end - start + 1) / MAX_THREADS;
    int remaining = (end - start + 1) % MAX_THREADS;
    int current_start = start;
    int current_end;

    for (int i = 0; i < MAX_THREADS; ++i) {
        current_end = current_start + range_size - 1;
        if (i < remaining) {
            current_end++;
        }
        thread_args[i][0] = current_start;
        thread_args[i][1] = current_end;
        pthread_create(&threads[i], NULL, count_primes, (void*)thread_args[i]);
        current_start = current_end + 1;
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        int* thread_count;
        pthread_join(threads[i], (void**)&thread_count);
        total_primes += *thread_count;
        free(thread_count);
    }

    gettimeofday(&end_time, NULL);
    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0; // Convertir a milisegundos
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    printf("Total primes between %d and %d: %d\n", start, end, total_primes);
    printf("Execution time: %.2f milliseconds\n", execution_time);

    return 0;
}
