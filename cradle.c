#include "cradle.h"

void GetToken()
{
    char temp[4];
    int length = 0;

    char ch = fgetc(fp);
    while(  ch != EOF   && ch != '\0' && 
            ch != (char)0x20  && ch != '\n' &&
            length < 4)
    {
        temp[length] = ch;
        ch = fgetc(fp);
        length++;
    }

    char *string = malloc(length * sizeof(char));
   
    /* 
     * Checks if string is actually a number
     */
    int isNumber = 1;
    int i;
    for(i = 0;i<length;i++)
    {
        if(temp[i] <= 47 ||
            temp[i] >= 58)
            isNumber = 0;
    }

    //Assumes number is in base 10
    if(isNumber == 0)
    {
        for(i = 0;i< length;i++)
            string[i] = UPCASE(temp[i]);
        
        Look->value.string = string;
        Look->strLength = length;
    }
    else
    {
        int tempNum = 0;
        for(i = 0;i<length;i++)
            tempNum = tempNum * 10 + temp[i] - 48;

        Look->value.number = tempNum;
        Look->strLength = 0;
    }
}

void Error(char *s)
{
    printf("\nError: %s",s);
}

void Expected(char *s)
{
    printf("\n Expected %s",s);
}

/*
 * Returns 1 if string cmp matches the string
 * in current Look token. String must be delimited
 * by newline or '\0'
 */
int TokenType(char* cmp)
{
    if(Look->strLength == 0)
        return 0;

    int i = 0;
    while(cmp[i] != '\0' && cmp[i] != '\n')
    {
        if(cmp[i] != Look->value.string[i])
            return 0;
        i++;
    }
    return 1;
}

/*
 * Returns true if current token is an operator
 */
int IsOperator()
{
    int i;
    for(i = 0;i<10;i++)
    {
        if(TokenType(OpList[i]) == 1)
            return 1;
    }
    return 0;
}

/*
 * Returns true if current token is a register, 5 
 * is number of registers
 */
int IsRegister()
{
    int i;
    for(i = 0;i < 5;i++)
    {
        if(TokenType(RegList[i]) == 1)
            return 1;
    }
    return 0;
}

/*
 * Returns true if current token is a number
 */
int IsNumber()
{
    if(Look->strLength == 0)
        return 1;
    return 0;
}

/*
 * Sets file pointer to instruction corresponding
 * to the instruction pointer
 */
void MoveFP(int insp)
{
    int InstrCount = 0;
    fseek(fp,0,SEEK_SET);
    while(InstrCount != insp + 1)
    {
        GetToken();
        if(IsOperator())
            InstrCount++;
        if(feof(fp))
        {
            Error("Jumped past EOF");
            break;
        }
    }
}

void Init(char *filename)
{
    Look = malloc(sizeof(token));

    OpList = (char**) malloc(10 * sizeof(char*));
    int i;
    for(i = 0;i < 10;i++)
        OpList[i] = (char*)malloc(3*sizeof(char));

    OpList[0] = "ADD";
    OpList[1] = "SUB";
    OpList[2] = "SHL";
    OpList[3] = "SHR";
    OpList[4] = "SET";
    OpList[5] = "AND";
    OpList[6] = "JMP";
    OpList[7] = "NOP";
    OpList[8] = "PRT";
    OpList[9] = "OR";

    RegList = (char**) malloc(5 * sizeof(char*));
    for(i = 0;i < 5;i++)
        RegList[i] = (char*)malloc(4*sizeof(char));

    RegList[0] = "REGA";
    RegList[1] = "REGB";
    RegList[2] = "REGC";
    RegList[3] = "REGX";
    RegList[4] = "INSP";
    
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        Error("No input file specified or file does not exist\n");
        exit(1);
    }
    GetToken();
}
