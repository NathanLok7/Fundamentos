#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Uso: %s <número de procesos>\n", argv[0]);
        return 0;
    }

    int n = atoi(argv[1]);

    for(int i = 1; i <= n; i++) {
        pid_t p = fork(); 

        if(p < 0) {
            perror("Error al crear el proceso hijo");
            return 0;
        }
        else if (p == 0) {
            printf("Proceso hijo %d\n", i);
            return 0;
        }
    }

    for(int i = 0; i < n; i++) {
        wait(NULL);
    }
  
    printf("Fin\n");
    return 0;
}
