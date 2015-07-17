#ifndef _CRADLE_H_
    #define _CRADLE_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ADD,SUB,SHL,SHR,SET,AND,JMP,NOP,PRT,OR
} Operators;

#define UPCASE(C) ((1<<6)|(C))

typedef struct {
    unsigned int strLength;
    union {
        char* string;
        int number;
    } value;
} token;


char **OpList;
char **RegList;
token *Look;
FILE *fp;

void GetToken();
void MoveFP(int insp);

void Error(char *s);
void Expected(char *s);
int TokenType(char* cmp);

int IsOperator();
int IsRegister();
int IsNumber();

void Init(char *filename);

#endif
