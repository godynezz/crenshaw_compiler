#include "./cradle.h"
#include <string.h>

void Block(char * L);


void Other() {
    sprintf(output_buffer, "<pseudo instruction>%c", getName());
    EmitLn(output_buffer);
}

void Condition(){
    EmitLn("<condition>");
}
void Expression(){
    EmitLn("<expr>");

}

void DoIf(char * L) {
    Match('i');
    Condition();

    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1,Newlabel());
    strcpy(L2,L1);

    sprintf(output_buffer, "BEQ %s", L1);
    EmitLn(output_buffer);
    
    Block(L);

    if(look == 'l'){
        Match('l');

        strcpy(L2,Newlabel());
        sprintf(output_buffer, "BRA %s", L2);
        EmitLn(output_buffer);
        postlabel(L1);
        Block(L);
    }

    Match('e');
    postlabel(L2);
}

void DoWhile(){
    Match('w');
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1,Newlabel());
    strcpy(L2,Newlabel());
    
    postlabel(L1);
    Condition();
    sprintf(output_buffer, "BEQ %s", L2);
    EmitLn(output_buffer);
    
    Block(L2);

    Match('e');
    sprintf(output_buffer, "BRA %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
}

void DoLoop(){
    Match('p');
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1,Newlabel());
    strcpy(L2,Newlabel());
    postlabel(L1);    
    Block(L2);
    sprintf(output_buffer, "BRA %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
}

void DoRepeat(){
    Match('r');
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1,Newlabel());
    strcpy(L2,Newlabel());

    postlabel(L1);

    Block(L2);
    Match('u');
    Condition();
    sprintf(output_buffer, "BEQ %s", L1);
    EmitLn(output_buffer);
    postlabel(L2);
}

void DoFor(){
    Match('f');
    char Name;
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    
    strcpy(L1,Newlabel());
    strcpy(L2,Newlabel());
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

void Dodo(){
    Match('d');
    
    char L1[L_BUFLEN];
    char L2[L_BUFLEN];
    strcpy(L1,Newlabel());
    strcpy(L2,Newlabel());
   

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

void DoBreak(char * L){
    Match('b');
    if (L == NULL) {
        Abort("No loop to break from");
    } else {
        sprintf(output_buffer, "BRA %s", L);
        EmitLn(output_buffer);
    }
}

void Block(char * L) {
    while (strchr("elu", look) == NULL) {
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
                Other();
                break;
        }
    }
}

void DoProgram() {
    Block(NULL);
    if (look != 'e') Expected("End");
    EmitLn("END");
}

int main(void) {
    Init();
    DoProgram();
    return EXIT_SUCCESS;
}