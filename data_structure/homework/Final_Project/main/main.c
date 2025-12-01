// main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calculator.h"

int main(void) {
    int mode = 0;
    char input[1024];

    // open log file
    FILE* f = fopen("result.log", "w");
    if (!f) {
        perror("Failed to open result.log");
        return 1;
    }
    set_log_file(f);

    printf("===== C DATA STRUCTURE CALCULATOR =====\n");
    printf("1) Infix\n");
    printf("2) Postfix\n");
    printf("3) Prefix\n");
    printf("4) Help\n");
    printf("Choose option (1-4): ");
    if (scanf("%d", &mode) != 1) {
        printf("Invalid selection\n");
        fclose(f);
        return 1;
    }
    // consume newline
    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    if (mode == 4) {
        show_help();
        fclose(f);
        return 0;
    }

    printf("Enter operation (use spaces or not; e.g. for postfix '3 4 2 * +' or '342*+'): \n> ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("No input\n");
        fclose(f);
        return 1;
    }
    // strip newline
    input[strcspn(input, "\r\n")] = '\0';

    fprintf(f, "User input mode=%d expr=%s\n", mode, input);
    fflush(f);

    int result = 0;
    if (mode == 1) {
        result = evaluate_infix(input);
    } else if (mode == 2) {
        result = evaluate_postfix(input);
    } else if (mode == 3) {
        result = evaluate_prefix(input);
    } else {
        printf("Unknown mode\n");
        fclose(f);
        return 1;
    }

    printf("Result: %d\n", result);
    fprintf(f, "Result: %d\n", result);
    fclose(f);
    printf("All steps saved to result.log\n");
    return 0;
}

