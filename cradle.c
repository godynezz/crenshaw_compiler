#include "./cradle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char look;
char output_buffer[BUFFER_SIZE];
char LebalName[L_BUFLEN];
int LCount;

void getChar(void) {
    look = getchar();
}

void Error(char *error) { printf("\n\033[1;31mError:\033[0m %s.\n", error); }

void Abort(char *error) {
    Error(error);
    exit(EXIT_FAILURE);
}

void Expected(char *expected) {
    sprintf(output_buffer, "%s Expected", expected);
    Abort(output_buffer);
}

void Match(char ch) {
    if (look != ch) {
        sprintf(output_buffer, "'%c'", ch);
        Expected(output_buffer);
    } else {
        getChar();
    }
}

int isAlpha(char ch) { return UPCASE(ch) >= 'A' && UPCASE(ch) <= 'Z'; }

int isDigit(char ch) { return ch >= '0' && ch <= '9'; }

int isAddop(char ch) { return ch == '+' || ch == '-'; }

int isAlNum(char ch) { return isAlpha(ch) || isDigit(ch); }

char getName(void) {
    char name;
    name = look;
    if (!isAlpha(look)) Expected("Name");
    getChar();
    return UPCASE(name);
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

void Emit(char *str) { printf("%c%s", HT, str); }

void EmitLn(char *str) { printf("%c%s\n", HT, str); }

void Init(void) {
    LCount = 0;
    getChar();
}


char *Newlabel(void) {
    sprintf(LebalName, "L%d", LCount);
    LCount += 1;
    return LebalName;
}

void postlabel(char *label) { printf("%s :\n", label); }

int IsBoolean(char ch) { return !!strchr("TF", UPCASE(ch)); }

int GetBoolean(void) {
    int c = look;
    getChar();
    if (!IsBoolean(c)) {
        Expected("Boolean Literal");
    }
    return UPCASE(c) == 'T';
}

int IsOrOp(char ch) { return !!strchr("|~", ch); }

int IsRelop(char ch) { return !!strchr("=#<>", ch); }

void Fin(void) {
    if (look == '\r') getChar();
    if (look == '\n') getChar();
}
