#include "./cradle.h"
int Expression(void);
int Term(void);
int Factor(void);

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


void Assignment(void){
    char Name;
    Name = getName();
    Match('=');
    Table[Name - 'A'] = Expression();
}


void Input(void){
    Match('?');
    char Name = getName();
    Table[Name - 'A'] = getNum(); 
}

void Output(void){
    Match('!');
    char Name = getName();
    printf("%c -> %d\n",Name, Table[Name - 'A']);
}

int main(void) {
    // a small terminal calculator
    // kinda feels like bc

    Init();

    while (look != '.') {
        switch (look) {
            case '?':
                Input();
                break;
            case '!':
                Output();
                break;
            default:
                Assignment();
                break;
        }
        NewLine();
    }

    return EXIT_SUCCESS;
}