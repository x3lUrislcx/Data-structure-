#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s CADENA\n", argv[0]);
        return -1;
    }

    char *cadena = argv[1];
    int len = strlen(cadena);

    int i = 0;
	for (i; i < len; i++) {
        char c = cadena[i];

        if (c < 'A' || c > 'Z') {
            printf("NULL\n");
            continue;
        }

        int repeticiones = (c - 'A') + 1;

        char *fila = (char *)malloc(repeticiones * sizeof(char));
        if (fila == NULL) {
            printf("Error: no se pudo asignar memoria\n");
            return -1;
        }

        int j = 0;
		for (j; j < repeticiones; j++) {
            fila[j] = c;
            printf("| %c ", fila[j]);
        }

        printf("|\n");

        free(fila);
    }

    printf("NULL\n");

    return 0;
}

