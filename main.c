#include <string.h>

#include "./cradle.h"

void Assignment(void);
void Expression(void);
void BoolExpression(void);

void Block(char* L);

void Other(void) {
    sprintf(output_buffer, "%c", getName());
    EmitLn(output_buffer);
}

void DoIf(char* L) {
    Match('i');
    BoolExpression();

    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1, Newlabel());
    strcpy(L2, L1);

    sprintf(output_buffer, "BEQ %s", L1);
    EmitLn(output_buffer);

    Block(L);

    if (look == 'l') {
        Match('l');

        strcpy(L2, Newlabel());
        sprintf(output_buffer, "BRA %s", L2);
        EmitLn(output_buffer);
        postlabel(L1);
        Block(L);
    }

    Match('e');
    postlabel(L2);
}

void DoWhile(void) {
    Match('w');
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1, Newlabel());
    strcpy(L2, Newlabel());

    postlabel(L1);
    BoolExpression();
    sprintf(output_buffer, "BEQ %s", L2);
    EmitLn(output_buffer);

    Block(L2);

    Match('e');
    sprintf(output_buffer, "BRA %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
}

void DoLoop(void) {
    Match('p');
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1, Newlabel());
    strcpy(L2, Newlabel());
    postlabel(L1);
    Block(L2);
    sprintf(output_buffer, "BRA %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
}

void DoRepeat(void) {
    Match('r');
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1, Newlabel());
    strcpy(L2, Newlabel());

    postlabel(L1);

    Block(L2);
    Match('u');
    BoolExpression();
    sprintf(output_buffer, "BEQ %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
}

void DoFor(void) {
    Match('f');
    char Name;
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];

    strcpy(L1, Newlabel());
    strcpy(L2, Newlabel());
    Name = getName();
    Match('=');
    Expression();

    EmitLn("SUBQ #1,D0");
    sprintf(output_buffer, "LEA %c (PC),A0", Name);
    EmitLn(output_buffer);
    EmitLn("MOVE D0,(A0)");
    Expression();
    EmitLn("MOVE D0,-(SP)");

    postlabel(L1);
    sprintf(output_buffer, "LEA %c (PC),A0", Name);
    EmitLn(output_buffer);
    EmitLn("MOVE (A0),D0");
    EmitLn("ADDQ #1,D0");
    EmitLn("MOVE D0,(A0)");
    EmitLn("CMP (SP),D0");
    sprintf(output_buffer, "BGT %s", L2);
    EmitLn(output_buffer);

    Block(L2);
    Match('e');
    sprintf(output_buffer, "BRA %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
    EmitLn("ADDRQ #2,SP");
}

void Dodo(void) {
    Match('d');

    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1, Newlabel());
    strcpy(L2, Newlabel());

    Expression();

    EmitLn("SUBQ #1,D0");
    postlabel(L1);
    EmitLn("MOVE D0,-(SP)");
    Block(L2);
    EmitLn("MOVE (SP)+,D0");

    sprintf(output_buffer, "DBRA D0, %s", L1);
    EmitLn(output_buffer);
    EmitLn("SUBQ #2,SP");
    postlabel(L2);
    EmitLn("ADDQ #2,SP");
}

void DoBreak(char* L) {
    Match('b');
    if (L == NULL) {
        Abort("No loop to break from");
    } else {
        sprintf(output_buffer, "BRA %s", L);
        EmitLn(output_buffer);
    }
}

void Block(char* L) {
    while (!strchr("elu", look)) {
        Fin();
        switch (look) {
            case 'i':
                DoIf(L);
                break;

            case 'w':
                DoWhile();
                break;

            case 'p':
                DoLoop();
                break;

            case 'r':
                DoRepeat();
                break;

            case 'f':
                DoFor();
                break;

            case 'd':
                Dodo();
                break;

            case 'b':
                DoBreak(L);
                break;

            default:
                Assignment();
                break;
        }
        Fin();
    }
}

void DoProgram(void) {
    Block(NULL);
    if (look != 'e') Expected("End");
    EmitLn("END");
}

////////////////////////////////////////////////////////////////////////////////
void Ident(void) {
    char Name = getName();
    if (look == '(') {
        Match('(');
        Match(')');
        sprintf(output_buffer, "BSR %c", Name);
        EmitLn(output_buffer);
    } else {
        sprintf(output_buffer, "MOVE %c(PC),D0", Name);
        EmitLn(output_buffer);
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
        sprintf(output_buffer, "MOVE #%d,D0", getNum());
        EmitLn(output_buffer);
    }
}

void SignedFactor(void) {
    if (look == '+') {
        getChar();
    }
    if (look == '-') {
        getChar();
        if (isDigit(look)) {
            sprintf(output_buffer, "MOVE #-%d,D0", getNum());
            EmitLn(output_buffer);
        } else {
            Factor();
            EmitLn("NEG D0");
        }
    } else {
        Factor();
    }
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
    EmitLn("EXS.L D0");
    EmitLn("DIVS D1,D0");
}

void Term(void) {
    SignedFactor();
    while (look == '*' || look == '/') {
        EmitLn("MOVE D0,-(SP)");
        if (look == '*') {
            Multiply();
        } else if (look == '/') {
            Divide();
        }
    }
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

void Expression(void) {
    Term();
    while (isAddop(look)) {
        EmitLn("MOVE D0,-(SP)");
        if (look == '+') {
            Add();
        } else if (look == '-') {
            Subtract();
        }
    }
}

// part 6

void Equals(void) {
    Match('=');
    Expression();
    EmitLn("CMP (SP)+,D0");
    EmitLn("SEQ D0");
}

void NotEquals(void) {
    Match('#');
    Expression();
    EmitLn("CMP (SP)+,D0");
    EmitLn("SNE D0");
}

void Less(void) {
    Match('<');
    Expression();
    EmitLn("CMP (SP)+,D0");
    EmitLn("SGE D0");
}

void Greater(void) {
    Match('>');
    Expression();
    EmitLn("CMP (SP)+,D0");
    EmitLn("SLE D0");
}

void Relation(void) {
    Expression();
    if (IsRelop(look)) {
        EmitLn("MOVE D0,-(SP)");
        switch (look) {
            case '=':
                Equals();
                break;
            case '#':
                NotEquals();
                break;
            case '<':
                Less();
                break;
            case '>':
                Greater();
                break;
        }
        EmitLn("TST D0");
    }
}

void BoolFactor(void) {
    if (IsBoolean(look)) {
        if (GetBoolean()) {
            EmitLn("MOVE #-1,D0");
        } else
            EmitLn("CLR D0");
    } else
        Relation();
}
void NotFactor(void) {
    if (look == '!') {
        Match('!');
        BoolFactor();
        EmitLn("EOR #-1,D0");
    } else{

        BoolFactor();
    }
}

void BoolTerm(void) {
    NotFactor();
    while (look == '&') {
        EmitLn("MOVE D0,-(SP)");
        Match('&');
        NotFactor();
        EmitLn("AND (SP)+,D0");
    }
}

void BoolOr(void) {
    Match('|');
    BoolTerm();
    EmitLn("OR (SP)+,D0");
}

void BoolXor(void) {
    Match('~');
    BoolTerm();
    EmitLn("EOR (SP)+,D0");
}

void BoolExpression(void) {
    BoolTerm();
    while (IsOrOp(look)) {
        EmitLn("MOVE D0,-(SP)");
        switch (look) {
            case '|':
                BoolOr();
                break;

            case '~':
                BoolXor();
                break;
        }
    }
}

void Assignment(void) {
    char Name = getName();
    Match('=');
    BoolExpression();
    sprintf(output_buffer, "LEA %c (PC),A0", Name);
    EmitLn(output_buffer);
    EmitLn("MOVE D0,(A0)");
}

int main(void) {
    Init();
    DoProgram();
    return EXIT_SUCCESS;
}
