#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <stdlib.h>

#define TAB '\t'
#define LINEFEED '\n'
#define CR '\r'
#define WHITESPACE ' '

#define TRUE 1
#define FALSE 0
#define UPCASE(C) (0xDF & C)  // clears the 6th bit
#define BUFFER_SIZE 1024      // Arbitrarily chosen number

char look;
char output_buffer[BUFFER_SIZE];
int Table[26];

// DEFINITION
void getChar(void);
void Error(char *error);
void Abort(char *error);
void Expected(char *expected);
void Match(char c);
int  isAlpha(char c);
int  isDigit(char c);
char getName(void);
int  getNum(void);
void Emit(char *str);
void EmitLn(char *str);
void Init(void);

int isAddop(char c);
void NewLine(void);

void InitTable(void);
void Input(void);
void Output(void);

void Assignment(void);
int Expression(void);
int Term(void);
int Factor(void);
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
    if (look != x) {
        sprintf(output_buffer, "'%c'", x);
        Expected(output_buffer);
    } else {
        getChar();
    }
}

int isAlpha(char x) {
    if (UPCASE(x) >= 'A' && UPCASE(x) <= 'Z') {
        return TRUE;
    } else
        return FALSE;
}

int isDigit(char x) {
    if (x >= '0' && x <= '9') {
        return TRUE;
    } else
        return FALSE;
}

int isAlNum(char c) { return (isAlpha(c) || isDigit(c)); }

char getName(void) {
    char c;
    if (!isAlpha(look)) Expected("Name");
    c = look;
    getChar();
    return UPCASE(c);
}

int getNum(void) {
    int value = 0;
    if (!isDigit(look)) Expected("Integer");

    while (isDigit(look)) {
        value = value * 10 + look - '0';
        getChar();
    }
    return value;
}

void Emit(char *str) { printf("%c%s", TAB, str); }

void EmitLn(char *str) { printf("%c%s\n", TAB, str); }

void Init(void) { getChar(); InitTable(); }

int isAddop(char c) {
    if (c == '+' || c == '-') {
        return TRUE;
    } else
        return FALSE;
}

void NewLine(){
    if (look == CR) {
        getChar();
    }
    if (look == LINEFEED) {
        getChar();
    }
}

void InitTable(void) {
    for (size_t i = 0; i < sizeof(Table) / sizeof(Table[0]); i++) {
        Table[i] = 0;
    }
}

void Input(){
    Match('?');
    Table[getName() - 'A'] = getNum();
}

void Output(){
    Match('!');
    printf("%d\n", Table[getName() - 'A']);
}

void Assignment(){
    char Name;
    Name = getName();
    Match('=');
    Table[Name - 'A'] = Expression();
}

int Factor(void) {
    int value = 0;
    if (look == '(') {
        Match('(');
        value = Expression();
        Match(')');
    } else if (isAlpha(look)) {
        return Table[getName() - 'A'];
    } else {
        return getNum();
    }
    return value;
}

int Term(void) {
    int value = Factor();

    while (look == '*' || look == '/') {
        switch (look) {
            case '*':
                Match('*');
                value = value * Factor();
                break;
            case '/':
                Match('/');
                value = value / Factor();
                break;
        }
    }
    return value;
}

int Expression(void) {
    int value;
    if (isAddop(look)) {
        value = 0;
    } else {
        value = Term();
    }

    while (isAddop(look)) {
        switch (look) {
            case '+':
                Match('+');
                value = value + Term();
                break;

            case '-':
                Match('-');
                value = value - Term();
        }
    }
    return value;
}

// The choice to do everything on the cradle header file is starting to bite my ass
#endif /* CRADLE_H */