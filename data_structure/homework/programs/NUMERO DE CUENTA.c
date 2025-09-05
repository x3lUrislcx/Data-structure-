#include <stdio.h>

int main() {
int pares[10];
int i;
int cuenta = 410092686; 
int ultimo = cuenta % 10; 


for (i = 0; i < 10; i++) {
pares[i] = (i + 1) * 2;
}

if (ultimo >= 0 && ultimo < 10) {
pares[ultimo] = -1;
}

printf("Lista en orden inverso:\n");
for (i = 9; i >= 0; i--) {
printf("%d ", pares[i]);
}

printf("\n");
return 0;
}
