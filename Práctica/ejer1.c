#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define N 1000000000 // 1'000,000'000 Número de iteraciones

double f(double y) {
    return sqrt(1 - y*y); // Función sqrt(1 - y^2)
}

double calcularPi() {
    double area = 0;
    double base, altura;

    double delta = 1.0 / N; // Tamaño de cada subintervalo

    for (int i = 0; i < N; i++) {
        base = f(i * delta); // Extremo izquierdo del trapecio
        base += f((i + 1) * delta); // Extremo derecho del trapecio
        altura = delta; // Altura del trapecio

        area += (base * altura) / 2; // Área del trapecio
    }

    return area * 4; // Multiplicamos por 4 para obtener el área del cuarto de círculo
}

int main() {
    struct timeval start, end;
    double tiempo;

    gettimeofday(&start, NULL); // Tiempo inicial

    double pi = calcularPi();

    gettimeofday(&end, NULL); // Tiempo final

    tiempo = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000; // Cálculo del tiempo transcurrido

    printf("Valor aproximado de PI: %.10f\n", pi);
    printf("Tiempo transcurrido: %.6f segundos\n", tiempo);
    
    return 0;
}
