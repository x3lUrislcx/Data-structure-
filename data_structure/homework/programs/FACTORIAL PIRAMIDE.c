#include <stdio.h>

// Recursive function to calculate the factorial of a number
long calculateFactorial(int n) {
    if (n <= 1) return 1;
    return n * calculateFactorial(n - 1);
}

// Function to display the pyramid of factorials
void printFactorialPyramid(int n) {
    printf("\nPyramid of Factorials:\n");
    printf("==================================\n\n");
    
    for (int i = n; i >= 1; i--) {
        // Spaces to center the pyramid
        for (int j = 0; j < (n - i); j++) {
            printf("   ");
        }
        
        // Numbers in the row
        for (int k = i; k >= 1; k--) {
            printf("%d ", k);
            if (k > 1) printf("* ");
        }
        
        // Result of the factorial
        printf("= %ld\n", calculateFactorial(i));
    }
}

int main() {
    int number;
    char continueCalculation;
    
    do {
        printf("FACTORIAL PYRAMID\n");
        printf("=================================\n\n");
        printf("Enter a number: ");
        scanf("%d", &number);
        
        // Clear the input buffer
        while(getchar() != '\n');
        
        if (number < 0) {
            printf("Error: Invalid number.\n");
        } else {
            printFactorialPyramid(number);
        }
        
        printf("\nDo you want to calculate another factorial? (y/n): ");
        scanf(" %c", &continueCalculation);
        
        // Clear the input buffer
        while(getchar() != '\n');
        
        printf("\n");
        
    } while(continueCalculation == 'y' || continueCalculation == 'Y');
    
    return 0;
}
