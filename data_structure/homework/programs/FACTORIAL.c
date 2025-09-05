#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int fact(int n){
	if(n == 0){
		return 1;
	}
	else if(n == 1){
		return 1;
	}
	else{
		return fact(n - 1)* n;
		
	}
}

int main (int argc, char*argv[]){
	int numero;
	printf("Ingresa un numero para calcular el factorial: " );
	scanf("%d", &numero);
	
	if(numero < 0){
		printf("Error ");
		return 1;
	}
	int result = fact(numero);
	printf("El factorial es: %d es: %d\n", numero, result);
	
    int i;
    char *num = argv[1];
    fprintf(stdout, "arg[1] = %s \n", argv[1]);
    
    for(i=0; i<strlen(argv[1]); i++)
        fprintf(stdout, "num[%d] = %c \n", i, num[i]);
        
    return 0;
}
