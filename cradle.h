#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <stdlib.h>

#define TAB '\t'
#define TRUE 1
#define FALSE 0
#define UPCASE(C) (0xDF & C)  // clears the 6th bit
#define BUFFER_SIZE 1024

char look;
char output_buffer[BUFFER_SIZE];

// DEFINITION
void getChar(void);
void Error(char *error);
void Abort(char *error);
void Expected(char *expected);
void Match(char x);
int isAlpha(char x);
int isDigit(char x);
char getName(void);
char getNum(void);
void Emit(char *str);
void EmitLn(char *str);
void Init(void);

void Expression(void);
void Factor(void);
void Term(void);
int isAddop(char c);

void Add(void);
void Subtract(void);
void Multiply(void);
void Divide(void);

// IMPLEMENTATION
void getChar(void) { look = getchar(); }
void Error(char *error) { printf("\nError: %s.\n", error); }

void Abort(char *error) {
    Error(error);
    exit(1);
}

void Expected(char *expected) {
    sprintf(output_buffer, "%s Expected", expected);
    Abort(output_buffer);
}

void Match(char x) {
    if (look == x) {
        getChar();
    } else {
        sprintf(output_buffer, "'%c'", x);
        Expected(output_buffer);
    }
}

int isAlpha(char x) {
    if (UPCASE(x) >= 'A' && UPCASE(x) <= 'Z') {
        return TRUE;
    } else {
        return FALSE;
    }
}

int isDigit(char x) {
    if (x >= '0' && x <= '9') {
        return TRUE;
    } else {
        return FALSE;
    }
}

char getName(void) {
    char cur = look;
    if (!isAlpha(cur)) {
        Expected("Name");
    }
    getChar();
    return UPCASE(cur);
}

char getNum(void) {
    char cur = look;
    if (!isDigit(cur)) {
        Expected("Integer");
    }
    getChar();
    return cur;
}

void Emit(char *str) { printf("%c%s", TAB, str); }

void EmitLn(char *str) { printf("%c%s\n", TAB, str); }

void Init(void) { getChar(); }

void Term() {
    Factor();
    while (look == '*' || look == '/') {
        EmitLn("MOVE D0,-(SP)");
        switch (look) {
            case '*':
                Multiply();
                break;
            case '/':
                Divide();
                break;
            default:
                Expected("Mulop");
                break;
        }
    }
}

void Expression() {
    if (isAddop(look)) {
        EmitLn("CLR D0");
    } else
        Term();

    while (isAddop(look)) {
        EmitLn("MOVE D0,-(SP)");
        switch (look) {
            case '+':
                Add();
                break;

            case '-':
                Subtract();
                break;

            default:
                Expected("addop");
                break;
        }
    }
}

void Factor() {
    if (look == '(') {
        Match('(');
        Expression();
        Match(')');
    } else {
        char num = getNum();
        sprintf(output_buffer, "MOVE #%c,D0", num);
        EmitLn(output_buffer);
    }
}

int isAddop(char c) {
    if (c == '+' || c == '-') {
        return TRUE;
    } else
        return FALSE;
}

void Add() {
    Match('+');
    Term();
    EmitLn("ADD (SP)+,D0");
}

void Subtract() {
    Match('-');
    Term();
    EmitLn("SUB (SP)+,D0");
    EmitLn("NEG D0");
}

void Multiply() {
    Match('*');
    Factor();
    EmitLn("MULS (SP)+,D0");
}

void Divide() {
    Match('/');
    Factor();
    EmitLn("MOVE (SP)+,D1");
    EmitLn("DIVS D1,D0");
}

#endif /* CRADLE_H */