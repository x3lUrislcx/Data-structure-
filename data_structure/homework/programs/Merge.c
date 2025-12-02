#include <stdio.h>

/* Merge */
void merge(int arr[], int left, int mid, int right, int asc) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[1000], R[1000];
    int i, j, k;

    for (i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }

    for (j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (asc == 1) {
            if (L[i] <= R[j]) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        } else {
            if (L[i] >= R[j]) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

/* MergeSort */
void mergeSort(int arr[], int left, int right, int asc) {
    if (left < right) {
        int mid = (left + right) / 2;

        mergeSort(arr, left, mid, asc);
        mergeSort(arr, mid + 1, right, asc);
        merge(arr, left, mid, right, asc);
    }
}

int main() {
    int n;
    int orden;
    int i;

    printf("\nMERGESORT\n");
    printf("¿Cuantos numeros deseas ingresar?: ");
    scanf("%d", &n);

    int arr[1000];

    printf("Ingresa los numeros:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Ordenar (1) Ascendente o (2) Descendente?: ");
    scanf("%d", &orden);

    mergeSort(arr, 0, n - 1, orden == 1 ? 1 : 0);

    printf("\nResultado ordenado:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}

