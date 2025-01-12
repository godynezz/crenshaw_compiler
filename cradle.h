#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <stdlib.h>

#define HT '\t'
#define LF '\n'
#define CR '\r'
#define SPACE ' '

#define TRUE 1
#define FALSE 0

// clears the 6th bit converting the character to it's uppercase equivalent
#define UPCASE(CH) (0xDF & CH)

#define BUFFER_SIZE 1024  // Arbitrarily chosen number
#define L_BUFLEN 24

extern char look;
extern char output_buffer[BUFFER_SIZE];
extern char LebalName[L_BUFLEN];
extern int LCount;

// DEFINITION
void getChar(void);
void Error(char *error);
void Abort(char *error);
void Expected(char *expected);
void Match(char c);
int isAlpha(char c);
int isDigit(char c);
char getName(void);
int getNum(void);
void Emit(char *str);
void EmitLn(char *str);

int isAddop(char c);

void Init(void);

int IsBoolean(char ch);
int GetBoolean(void);

int IsOrOp(char ch);
int IsRelop(char ch);

char *Newlabel(void);
void postlabel(char *label);

void Fin(void);
#endif /* CRADLE_H */
