#include <stdio.h>

unsigned long long factorial_recursivo(int n) {
    if (n == 0 || n == 1) {
        return 1; 
    } else {
        return n * factorial_recursivo(n - 1); 
    }
}

unsigned long long factorial_iterativo(int n) {
    unsigned long long resultado = 1;
    int i=2;
    for (i; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

int main() {
    int n, opcion;
    
    printf("=== Calculo de Factorial ===\n");
    printf("1. Usar recursion\n");
    printf("2. Usar iteracion\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);   

    printf("Ingrese un numero: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("El factorial no esta definido para numeros negativos.\n");
    } else {
        if (opcion == 1) {
            printf("El factorial de %d (recursivo) es: %llu\n", n, factorial_recursivo(n));
        } else if (opcion == 2) {
            printf("El factorial de %d (iterativo) es: %llu\n", n, factorial_iterativo(n));
        } else {
            printf("Opcion no valida.\n");
        }
    }

    return 0;
}

