// calculator.c
#define _GNU_SOURCE
#include "calculator.h"
#include "Stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static FILE* logFile = NULL;

void set_log_file(FILE* f) {
    logFile = f;
}

static void log_step(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (logFile) {
        vfprintf(logFile, fmt, ap);
        fprintf(logFile, "\n");
        fflush(logFile);
    }
    va_end(ap);
}

// ------------------------- utilities -------------------------
static int is_operator_char(const char c) {
    return c=='+'||c=='-'||c=='*'||c=='/'||c=='^';
}

static int precedence(char op) {
    if (op == '^') return 4;
    if (op == '*' || op == '/') return 3;
    if (op == '+' || op == '-') return 2;
    return 0;
}

static int apply_op(int a, int b, char op, int* ok) {
    *ok = 1;
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) { *ok = 0; return 0; }
        return a / b;
    }
    if (op == '^') {
        int res = 1;
        for (int i = 0; i < b; ++i) res *= a;
        return res;
    }
    *ok = 0;
    return 0;
}

// Token structure
typedef struct {
    char* text; // malloc'd
} Token;

static void free_tokens(Token* toks, int n) {
    for (int i = 0; i < n; ++i) free(toks[i].text);
    free(toks);
}

// Tokenize expression (numbers, letters, operators, parentheses).
// Returns dynamic array of Token and sets *count. Caller must free_tokens().
static Token* tokenize(const char* s, int* count) {
    int len = strlen(s);
    Token* arr = NULL;
    int cap = 0, n = 0;
    int i = 0;
    while (i < len) {
        if (isspace((unsigned char)s[i])) { i++; continue; }
        if (isdigit((unsigned char)s[i])) {
            int j = i;
            while (j < len && isdigit((unsigned char)s[j])) j++;
            int L = j - i;
            char* t = (char*)malloc(L + 1);
            memcpy(t, s + i, L);
            t[L] = '\0';
            if (n >= cap) { cap = cap ? cap*2 : 8; arr = realloc(arr, cap * sizeof(Token)); }
            arr[n++].text = t;
            i = j;
            continue;
        }
        if (isalpha((unsigned char)s[i])) {
            // treat single letter as a token (A, B, ...)
            char* t = (char*)malloc(2);
            t[0] = s[i];
            t[1] = '\0';
            if (n >= cap) { cap = cap ? cap*2 : 8; arr = realloc(arr, cap * sizeof(Token)); }
            arr[n++].text = t;
            i++;
            continue;
        }
        // operators or parentheses (single char)
        char* t = (char*)malloc(2);
        t[0] = s[i];
        t[1] = '\0';
        if (n >= cap) { cap = cap ? cap*2 : 8; arr = realloc(arr, cap * sizeof(Token)); }
        arr[n++].text = t;
        i++;
    }
    *count = n;
    return arr;
}

// Collect variable letters and ask user for their numeric values.
// Returns an integer map: for letters 'A'..'Z' store values in array vals and flags in present.
// The program will prompt for each distinct letter found.
static void get_variable_values_if_any(Token* toks, int ntoks, int vals[26], int present[26]) {
    for (int i = 0; i < 26; ++i) { present[i] = 0; vals[i] = 0; }
    for (int i = 0; i < ntoks; ++i) {
        char* t = toks[i].text;
        if (strlen(t) == 1 && isalpha((unsigned char)t[0])) {
            int idx = toupper((unsigned char)t[0]) - 'A';
            present[idx] = 1;
        }
    }
    for (int i = 0; i < 26; ++i) {
        if (present[i]) {
            printf("Enter value for %c: ", 'A' + i);
            int v = 0;
            if (scanf("%d", &v) != 1) {
                // if failed, set zero
                v = 0;
            }
            vals[i] = v;
            if (logFile) fprintf(logFile, "Variable %c = %d\n", 'A'+i, v);
        }
    }
}

// ------------------------- Convert infix -> postfix (shunting-yard) -------------------------
static Token* infix_to_postfix(Token* inToks, int nIn, int* outCount, int* ok) {
    *ok = 1;
    Stack opStack; initStack(&opStack);
    Token* out = NULL;
    int cap = 0, nOut = 0;

    for (int i = 0; i < nIn; ++i) {
        char* t = inToks[i].text;
        if (isdigit((unsigned char)t[0]) || (strlen(t)==1 && isalpha((unsigned char)t[0]))) {
            // operand -> output
            if (nOut >= cap) { cap = cap ? cap*2 : 8; out = realloc(out, cap * sizeof(Token)); }
            out[nOut++].text = strdup(t);
            log_step("Output operand: %s", t);
        } else if (is_operator_char(t[0]) && strlen(t)==1) {
            char op = t[0];
            // while top of opStack is operator with greater or equal precedence
            while (!isEmpty(&opStack)) {
                char* top = peek(&opStack);
                if (top && strlen(top)==1 && is_operator_char(top[0]) &&
                    ((precedence(top[0]) > precedence(op)) || (precedence(top[0]) == precedence(op) && op != '^'))) {
                    // pop to output
                    char* popped = pop(&opStack);
                    if (nOut >= cap) { cap = cap ? cap*2 : 8; out = realloc(out, cap * sizeof(Token)); }
                    out[nOut++].text = popped; // already malloc'd
                    log_step("Pop op '%s' to output", out[nOut-1].text);
                    free(top);
                } else {
                    free(top);
                    break;
                }
            }
            // push current op
            char s[2] = {op, '\0'};
            push(&opStack, s);
            log_step("Push operator: %c", op);
        } else if (strcmp(t, "(") == 0) {
            push(&opStack, "(");
            log_step("Push '(' to op stack");
        } else if (strcmp(t, ")") == 0) {
            // pop until '('
            int found = 0;
            while (!isEmpty(&opStack)) {
                char* top = pop(&opStack);
                if (strcmp(top, "(") == 0) {
                    free(top);
                    found = 1;
                    log_step("Found '(' and removed");
                    break;
                } else {
                    // move to output
                    if (nOut >= cap) { cap = cap ? cap*2 : 8; out = realloc(out, cap * sizeof(Token)); }
                    out[nOut++].text = top; // already malloc'd
                    log_step("Pop op '%s' to output while looking for '('", out[nOut-1].text);
                }
            }
            if (!found) {
                *ok = 0;
                log_step("Syntax error: mismatched parentheses");
                freeStack(&opStack);
                if (out) free_tokens(out, nOut);
                return NULL;
            }
        } else {
            // unknown token
            *ok = 0;
            log_step("Syntax error: unknown token '%s'", t);
            freeStack(&opStack);
            if (out) free_tokens(out, nOut);
            return NULL;
        }
    }

    // pop remaining operators
    while (!isEmpty(&opStack)) {
        char* top = pop(&opStack);
        if (strcmp(top, "(") == 0 || strcmp(top, ")") == 0) {
            *ok = 0;
            log_step("Syntax error: mismatched parentheses at finalization");
            free(top);
            freeStack(&opStack);
            if (out) free_tokens(out, nOut);
            return NULL;
        }
        if (nOut >= cap) { cap = cap ? cap*2 : 8; out = realloc(out, cap * sizeof(Token)); }
        out[nOut++].text = top; // already malloc'd
        log_step("Pop remaining op '%s' to output", out[nOut-1].text);
    }

    freeStack(&opStack);
    *outCount = nOut;
    return out;
}

// ------------------------- Evaluate postfix tokens -------------------------
static int evaluate_postfix_tokens(Token* toks, int ntoks, int varvals[26], int varpresent[26], int* ok) {
    *ok = 1;
    Stack st; initStack(&st);
    for (int i = 0; i < ntoks; ++i) {
        char* t = toks[i].text;
        if (isdigit((unsigned char)t[0])) {
            push(&st, t);
            log_step("PUSH number '%s' onto evaluation stack", t);
        } else if (strlen(t)==1 && isalpha((unsigned char)t[0])) {
            int idx = toupper((unsigned char)t[0]) - 'A';
            int v = varpresent[idx] ? varvals[idx] : 0;
            char buf[64]; sprintf(buf, "%d", v);
            push(&st, buf);
            log_step("PUSH variable '%s' value %d onto evaluation stack", t, v);
        } else if (is_operator_char(t[0]) && strlen(t)==1) {
            // pop b then a
            char* sb = pop(&st);
            char* sa = pop(&st);
            if (!sa || !sb) {
                *ok = 0;
                log_step("Syntax error: not enough operands for operator '%s'", t);
                if (sa) free(sa);
                if (sb) free(sb);
                freeStack(&st);
                return 0;
            }
            int a = atoi(sa);
            int b = atoi(sb);
            free(sa); free(sb);
            int iok=1;
            int r = apply_op(a, b, t[0], &iok);
            if (!iok) {
                *ok = 0;
                log_step("Math error (division by zero or unknown op) while applying '%s'", t);
                freeStack(&st);
                return 0;
            }
            char buf[64]; sprintf(buf, "%d", r);
            push(&st, buf);
            log_step("APPLY %d %s %d = %d, push result", a, t, b, r);
        } else {
            *ok = 0;
            log_step("Unknown token in evaluation: '%s'", t);
            freeStack(&st);
            return 0;
        }
    }

    char* final = pop(&st);
    if (!final || !isEmpty(&st)) {
        *ok = 0;
        log_step("Syntax error: evaluation did not finish cleanly");
        if (final) free(final);
        freeStack(&st);
        return 0;
    }
    int result = atoi(final);
    log_step("Final result: %d", result);
    free(final);
    freeStack(&st);
    return result;
}

// ------------------------- Public functions -------------------------

void show_help() {
    printf("Calculator Help:\n");
    printf(" - Supports: infix (e.g. 3 + 4 * 2), postfix (e.g. 3 4 2 * +), prefix (e.g. + 3 * 4 2)\n");
    printf(" - Multi-digit integers supported (positive). Use letters (A,B,...) as variables; you'll be prompted for their values.\n");
    printf(" - Operators: + - * / ^ and parentheses ( ) for infix.\n");
    printf(" - All steps are recorded to result.log.\n");
    printf("\nExample infix: (A + 3) * (B - 2)\n");
}

int evaluate_infix(const char* expr) {
    log_step("=== INFIX EVALUATION START ===");
    int nIn = 0;
    Token* inToks = tokenize(expr, &nIn);
    int varvals[26], varpresent[26];
    get_variable_values_if_any(inToks, nIn, varvals, varpresent);

    int okConv = 0;
    int ntoksOut = 0;
    Token* postfix = infix_to_postfix(inToks, nIn, &ntoksOut, &okConv);

    // log postfix expression
    if (!okConv || !postfix) {
        log_step("Infix -> Postfix conversion failed (syntax error).");
        free_tokens(inToks, nIn);
        return 0;
    }
    // build a printable postfix string
    {
        char buf[1024] = {0};
        int pos = 0;
        for (int i = 0; i < ntoksOut; ++i) {
            pos += snprintf(buf+pos, sizeof(buf)-pos, "%s ", postfix[i].text);
        }
        log_step("Postfix form: %s", buf);
    }

    int okEval = 0;
    int result = evaluate_postfix_tokens(postfix, ntoksOut, varvals, varpresent, &okEval);

    if (!okEval) {
        log_step("Evaluation failed (syntax/math error).");
    } else {
        log_step("Infix evaluation result = %d", result);
    }

    free_tokens(inToks, nIn);
    free_tokens(postfix, ntoksOut);
    log_step("=== INFIX EVALUATION END ===");
    return result;
}

int evaluate_postfix(const char* expr) {
    log_step("=== POSTFIX EVALUATION START ===");
    int nT = 0;
    Token* toks = tokenize(expr, &nT);
    int varvals[26], varpresent[26];
    get_variable_values_if_any(toks, nT, varvals, varpresent);

    int ok = 0;
    int result = evaluate_postfix_tokens(toks, nT, varvals, varpresent, &ok);
    if (!ok) log_step("Postfix evaluation ended with error.");
    else log_step("Postfix evaluation result = %d", result);

    free_tokens(toks, nT);
    log_step("=== POSTFIX EVALUATION END ===");
    return result;
}

// For prefix: convert to tokens, evaluate from right-to-left similarly
int evaluate_prefix(const char* expr) {
    log_step("=== PREFIX EVALUATION START ===");
    int nT = 0;
    Token* toks = tokenize(expr, &nT);
    int varvals[26], varpresent[26];
    get_variable_values_if_any(toks, nT, varvals, varpresent);

    // We will evaluate by scanning from right to left using a stack
    Stack st; initStack(&st);
    int ok = 1;
    for (int i = nT - 1; i >= 0; --i) {
        char* t = toks[i].text;
        if (isdigit((unsigned char)t[0])) {
            push(&st, t);
            log_step("PUSH number '%s' (prefix)", t);
        } else if (strlen(t)==1 && isalpha((unsigned char)t[0])) {
            int idx = toupper((unsigned char)t[0]) - 'A';
            int v = varpresent[idx] ? varvals[idx] : 0;
            char buf[64]; sprintf(buf, "%d", v);
            push(&st, buf);
            log_step("PUSH variable '%s' value %d (prefix)", t, v);
        } else if (is_operator_char(t[0]) && strlen(t)==1) {
            char* sa = pop(&st);
            char* sb = pop(&st);
            if (!sa || !sb) {
                ok = 0;
                log_step("Syntax error: not enough operands for operator '%s' in prefix", t);
                if (sa) free(sa); if (sb) free(sb);
                freeStack(&st);
                break;
            }
            int a = atoi(sa);
            int b = atoi(sb);
            free(sa); free(sb);
            int iok = 1;
            int r = apply_op(a, b, t[0], &iok);
            if (!iok) {
                ok = 0;
                log_step("Math error in prefix evaluation while applying '%s'", t);
                freeStack(&st);
                break;
            }
            char buf[64]; sprintf(buf, "%d", r);
            push(&st, buf);
            log_step("APPLY (prefix) %d %s %d = %d, push result", a, t, b, r);
        } else {
            ok = 0;
            log_step("Unknown token '%s' in prefix evaluation", t);
            freeStack(&st);
            break;
        }
    }

    int result = 0;
    if (ok) {
        char* final = pop(&st);
        if (!final || !isEmpty(&st)) {
            ok = 0;
            log_step("Syntax error: prefix evaluation did not finish cleanly");
            if (final) free(final);
            freeStack(&st);
        } else {
            result = atoi(final);
            log_step("Prefix result = %d", result);
            free(final);
        }
    }

    freeStack(&st);
    free_tokens(toks, nT);
    log_step("=== PREFIX EVALUATION END ===");
    return result;
}

