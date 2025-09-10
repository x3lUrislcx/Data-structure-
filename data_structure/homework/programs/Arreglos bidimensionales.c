#include <stdio.h>
#include <stdlib.h>  
#include <string.h>  

#define MAX 100   
#define COLS 3    
#define SIZE 25   

int main() {
    char alumnos[MAX][COLS][SIZE];
    int n = 0;
    char opcion;

    printf("=== Registro de Alumnos ===\n");

    do {
        if (n >= MAX) {
            printf("Se alcanzo el máximo de alumnos (%d).\n", MAX);
            break;
        }

        printf("\nAlumno #%d:\n", n + 1);
        printf("Nombre: ");
        fgets(alumnos[n][0], SIZE, stdin);
        alumnos[n][0][strcspn(alumnos[n][0], "\n")] = 0; 

        printf("Edad: ");
        fgets(alumnos[n][1], SIZE, stdin);
        alumnos[n][1][strcspn(alumnos[n][1], "\n")] = 0;

        printf("Calificacion: ");
        fgets(alumnos[n][2], SIZE, stdin);
        alumnos[n][2][strcspn(alumnos[n][2], "\n")] = 0;

        n++;

        printf("\n¿Desea agregar otro alumno? (s/n): ");
        opcion = getchar();
        while (getchar() != '\n'); 
    } while (opcion == 's' || opcion == 'S');

    int sumaEdad = 0, sumaCalif = 0;
    int i = 0;
	for (i; i < n; i++) {
        sumaEdad += atoi(alumnos[i][1]);
        sumaCalif += atoi(alumnos[i][2]);
    }
    float promEdad = (float)sumaEdad / n;
    float promCalif = (float)sumaCalif / n;

    printf("\n=== Resultados ===\n");
    printf("Promedio de edad: %.2f\n", promEdad);
    printf("Promedio de calificación: %.2f\n", promCalif);

    printf("\nNombres en orden inverso:\n");
    int j = n;
	for (j - 1; j >= 0; j--) {
        printf("%s\n", alumnos[i][0]);
    }

    return 0;
}

