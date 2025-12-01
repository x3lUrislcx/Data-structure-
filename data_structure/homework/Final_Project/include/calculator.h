// calculator.h
#ifndef CALCULATOR_H
#define CALCULATOR_H

void show_help();
int evaluate_infix(const char* expr);
int evaluate_postfix(const char* expr);
int evaluate_prefix(const char* expr);

#endif

