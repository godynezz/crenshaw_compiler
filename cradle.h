#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <stdlib.h>

#define TAB '\t'
#define NEWLINE '\n'
#define White ' '
#define TRUE 1
#define FALSE 0
#define UPCASE(C) (0xDF & C)  // clears the 6th bit
#define BUFFER_SIZE 1024      // Arbitrarily chosen number
#define MAX_TOKEN 32          // Arbitrarily chosen number

char look;
char output_buffer[BUFFER_SIZE];
char token_buffer[MAX_TOKEN];

// DEFINITION
// part 1
void getChar(void);
void Error(char *error);
void Abort(char *error);
void Expected(char *expected);
void Match(char c);
int isAlpha(char c);
int isDigit(char c);
char *getName(void);
char *getNum(void);
void Emit(char *str);
void EmitLn(char *str);
void Init(void);

// part 2
void Expression(void);
void Factor(void);
void Term(void);
int isAddop(char c);

void Add(void);
void Subtract(void);
void Multiply(void);
void Divide(void);

// part 3
void Ident(void);
void Assignment(void);
int isAlNum(char c);
int isWhite(char c);
void SkipWhite();

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
    if (look != x) {
        sprintf(output_buffer, "'%c'", x);
        Expected(output_buffer);
    } else {
        getChar();
        SkipWhite();
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

char *getName(void) {
    char *token = calloc(MAX_TOKEN, sizeof(char));
    int size;
    if (!isAlpha(look)) Expected("Name");

    for (int index = 0; isAlNum(look); index++) {
        if (index >= MAX_TOKEN - 1) {
            Abort("token overflow");
        }
        token[index] = UPCASE(look);
        size = index + 1;
        getChar();
    }
    token[size] = '\0';

    SkipWhite();
    return token;
}

char *getNum(void) {
    char *Value = calloc(MAX_TOKEN, sizeof(char));
    int size;
    if (!isDigit(look)) Expected("Integer");

    for (int index = 0; isAlNum(look); index++) {
        if (index >= MAX_TOKEN - 1) {
            Abort("token overflow");
        }
        Value[index] = look;
        size = index + 1;
        getChar();
    }
    Value[size] = '\0';


    SkipWhite();
    return Value;
}

void Emit(char *str) { printf("%c%s", TAB, str); }

void EmitLn(char *str) { printf("%c%s\n", TAB, str); }

void Init(void) { getChar(); }

void Term(void) {
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

void Expression(void) {
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

void Factor(void) {
    if (look == '(') {
        Match('(');
        Expression();
        Match(')');
    } else if (isAlpha(look)) {
        Ident();
    } else {
        char *num = getNum();
        sprintf(output_buffer, "MOVE #%s,D0", num);
        EmitLn(output_buffer);
        free(num);
    }
}

int isAddop(char c) {
    if (c == '+' || c == '-') {
        return TRUE;
    } else
        return FALSE;
}

void Add(void) {
    Match('+');
    Term();
    EmitLn("ADD (SP)+,D0");
}

void Subtract(void) {
    Match('-');
    Term();
    EmitLn("SUB (SP)+,D0");
    EmitLn("NEG D0");
}

void Multiply(void) {
    Match('*');
    Factor();
    EmitLn("MULS (SP)+,D0");
}

void Divide(void) {
    Match('/');
    Factor();
    EmitLn("MOVE (SP)+,D1");
    EmitLn("DIVS D1,D0");
}

void Ident(void) {
    char *name = getName();
    if (look == '(') {
        Match('(');
        Match(')');
        sprintf(output_buffer, "BSR %s", name);
        free(name);
        EmitLn(output_buffer);
    } else {
        sprintf(output_buffer, "MOVE %s(PC),D0", name);
        EmitLn(output_buffer);
        free(name);
    }

}

void Assignment(void) {
    char *name = getName();
    Match('=');
    Expression();

    sprintf(output_buffer, "LEA %s(PC),A0", name);
    EmitLn(output_buffer);

    free(name);
    EmitLn("MOVE D0,(A0)");
}

int isWhite(char c) {
    if (c == White || c == TAB) {
        return TRUE;
    } else
        return FALSE;
}

void SkipWhite() {
    while (isWhite(look)) getChar();
}

#endif /* CRADLE_H */