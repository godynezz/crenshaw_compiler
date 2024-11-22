#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <stdlib.h>

#define TAB '\t'
#define TRUE 1
#define FALSE 0
#define UPCASE(C) (0xDF & (C))
#define BUFFER_SIZE 1024

char look;
char output_buffer[BUFFER_SIZE];

void getChar(void) {
    look = getchar();
}

void Error(char *error) {
    printf("\nError: %s.\n", error);
}

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

char getname(void) {
    if (!isAlpha(look)) {
        Expected("Name");
    }
    getChar();
    return UPCASE(look);
}

char getNum(void) {
    if (!isDigit(look)) {
        Expected("Integer");
    }
    return look;
}

void Emit(char *str) {
    printf("%c%s", TAB, str);
}

void EmitLn(char *str) {
    printf("%c%s\n", TAB, str);
}

void Init(void) {
    getChar();
}

#endif /* CRADLE_H */