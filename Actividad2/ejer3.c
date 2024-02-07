#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char programa[1000];

    while (1) {
        printf("Introduce el nombre del programa ('exit' para salir): ");
        if (fgets(programa, sizeof(programa), stdin) == NULL) {
            // Manejo de error
            return 0;
        }
        size_t len = strlen(programa);
        if(len > 0 && programa[len - 1] == '\n') {
            programa[len - 1] = '\0';
        }

        if(strcmp(programa, "exit") == 0) {
            printf("Saliendo del programa.\n");
            break;
        }

        int resultado = system(programa);
        if(resultado == 0) {
            printf("Programa ejecutado correctamente.\n");
        } 
        else{
            printf("Error al ejecutar el programa.\n");
        }
    }
    return 0;
}
