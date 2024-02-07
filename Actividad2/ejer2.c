
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int nivel = atoi(argv[1]);

    printf("%d\n", 0);
    for(int i = 1; i <= nivel; i++) {
        fork();
        printf("%d\n", i);
    }
    return 0;
}
