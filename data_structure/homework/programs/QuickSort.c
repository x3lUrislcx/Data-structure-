#include <stdio.h>

/* Intercambio */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Partición */
int partition(int arr[], int low, int high, int asc) {
    int pivot = arr[high];
    int i = low - 1;
    int j;

    for (j = low; j < high; j++) {
        if (asc == 1) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        } else {
            if (arr[j] > pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* QuickSort */
void quickSort(int arr[], int low, int high, int asc) {
    if (low < high) {
        int pi = partition(arr, low, high, asc);
        quickSort(arr, low, pi - 1, asc);
        quickSort(arr, pi + 1, high, asc);
    }
}

int main() {
    int n;
    int orden;
    int i;

    printf("\nQUICKSORT\n");
    printf("¿Cuantos numeros deseas ingresar?: ");
    scanf("%d", &n);

    int arr[1000]; /* Espacio suficiente */

    printf("Ingresa los numeros:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Ordenar (1) Ascendente o (2) Descendente?: ");
    scanf("%d", &orden);

    quickSort(arr, 0, n - 1, orden == 1 ? 1 : 0);

    printf("\nResultado ordenado:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}

