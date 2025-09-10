#include <stdio.h>

int main() {
    int n, m;
    int matriz[10][10];

    printf("Ingrese el número de filas (máx 10): ");
    scanf("%d", &n);
    printf("Ingrese el número de columnas (máx 10): ");
    scanf("%d", &m);

    if (n > 10 || m > 10 || n <= 0 || m <= 0) {
        printf("Dimensiones inválidas. Deben ser entre 1 y 10.\n");
        return 1;
    }

    printf("Ingrese los elementos de la matriz (%d x %d):\n", n, m);
    int i = 0;
	for (i; i < n; i++) {
        int j = 0;
		for (j; j < m; j++) {
            printf("Elemento [%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    printf("\nRecorrido por filas:\n");
    int o = 0;
	for (o; o < n; o++) {
        int j = 0;
		for (j; j < m; j++) {
            printf("%d ", matriz[i][j]);
        }
    }
    printf("\n");

    printf("\nRecorrido por columnas (de derecha a izquierda):\n");
    int j = m;
	for (j - 1; j >= 0; j--) {
        int i = 0;
		for (i; i < n; i++) {
            printf("%d ", matriz[i][j]);
        }
    }
    printf("\n");

    printf("\nRecorrido diagonal:\n");
    int limite = (n < m) ? n : m; 
    int z = 0;
	for (z; z < limite; z++) {
        printf("%d ", matriz[i][i]);
    }
    printf("\n");

    return 0;
}

