#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
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
    int start = *((int*)arg);
    int end = start + 2500000; // Each thread processes 2,500,000 numbers
    int count = 0;

    for (int i = start; i < end; ++i) {
        if (is_prime(i)) {
            count++;
        }
    }

    return (void*)(intptr_t)count;
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        return 1;
    }

    int start, end;
    printf("Enter the range (start end): ");
    scanf("%d %d", &start, &end);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    int total_primes = 0;
    pthread_t threads[MAX_THREADS];
    int thread_args[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; ++i) {
        thread_args[i] = start + i * 2500000;
        pthread_create(&threads[i], NULL, count_primes, (void*)&thread_args[i]);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        int* thread_count;
        pthread_join(threads[i], (void**)&thread_count);
        total_primes += *thread_count;
    }

    gettimeofday(&end_time, NULL);
    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0; // Convert to milliseconds
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    printf("Total primes between %d and %d: %d\n", start, end, total_primes);
    printf("Execution time: %.2f milliseconds\n", execution_time);

    return 0;
}
