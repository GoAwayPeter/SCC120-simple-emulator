#include "cradle.h"

int Register[5] = {0,0,0,0,0};

#define REGA Register[0]
#define REGB Register[1]
#define REGC Register[2]
#define REGX Register[3]
#define INSP Register[4]

/* 
 * Register operations happen here
 *
 */
void Operation(Operators op)
{
    GetToken();
    int i;
    if(IsRegister())
    {
        for(i = 0;i < 5;i++)
        {
            if(TokenType(RegList[i]))
                break;
        }
    }
    else
    {
        printf("Expected register, not %s or %d\n",
                Look->value.string, Look->value.number);
        exit(1);
    }
    GetToken();
    if(IsNumber())
    {
        switch(op)
        {
            case ADD:
                Register[i] += Look->value.number;
                break;
            case SUB:
                Register[i] -= Look->value.number;
                break;
            case SHL:
                Register[i] = Register[i] << Look->value.number;
                break;
            case SHR:
                Register[i] = Register[i] >> Look->value.number;
                break;
            case SET:
                Register[i] = Look->value.number;
                break;
            case AND:
                Register[i] = Register[i] & Look->value.number;
                break;
            case OR:
                Register[i] = Register[i] | Look->value.number;
                break;
            default:
                printf(" opcode: %d",op);
        }
    }
    else if(IsRegister())
    {
        int j;
        for(j = 0;j < 5;j++)
        {
            if(TokenType(RegList[j]))
            {
                switch(op)
                {
                    case ADD:
                        Register[i] += Register[j];
                        break;
                    case SUB:
                        Register[i] -= Register[j];
                        break;
                    case SHL:
                        Register[i] = Register[i] << Register[j];
                        break;
                    case SHR:
                        Register[i] = Register[i] >> Register[j];
                        break;
                    case SET:
                        Register[i] = Register[j];
                        break;
                    case AND:
                        Register[i] = Register[i] & Register[j];
                        break;
                    case OR:
                        Register[i] = Register[i] | Register[j];
                        break;
                    default:
                        printf(" opcode: %d",op);
                }
            }
        }
    }
    else
    {
        Expected(" number or register, you dun goofed");
        exit(1);
    }
    
    INSP++;
    GetToken();
}

void Jmp()
{
    GetToken();
    if(IsNumber())
    {
        if(REGX == 0)
            MoveFP(Look->value.number);
        else
            GetToken();
    }
    else if(IsRegister())
    {
        if(REGX == 0)
        {
            int i;
            for(i = 0;i < 5;i++)
            {
                if(TokenType(RegList[i]))
                    MoveFP(Register[i]);
            }
        }
        else
            GetToken();
    }
    else
    {
        Expected(" number or register, cannot jump to operator");
        exit(1);
    }
}

void Nop()
{
    INSP++;
    GetToken();
}

void Prt()
{
    GetToken();
    if(IsRegister())
    {
        int i;
        for(i = 0;i < 5;i++)
        {
            if(TokenType(RegList[i]))
                printf("%s: %d \n",RegList[i],Register[i]); 
        }
    }
    else
    {
        Expected(" register, you dun goofed");
        exit(1);
    }
    INSP++;
    GetToken();
}

/* 
 * Array of function pointers
 */
void (*FuncPt[3])() = {Jmp, Nop, Prt};

/*
 * Evaluates operator, and then calls 
 * appropriate function
 */
void Expression()
{
    while(IsOperator())
    {
        int i;
        for(i = 0;i < 10;i++)
        {
            if(TokenType(OpList[i]))
            {
                if((i >= 0 && i <= 5) || i == 9)
                    Operation((Operators)i);
                else
                    (*FuncPt[i - 6])();
                break;
            }
        }
    }
}

void printTheProgram(char* argv)
{
    int c;
    int lineNum = 0;
    FILE *file;
    file = fopen(argv, "r");
    printf("%d :",lineNum);
    if (file) 
    {
        while ((c = getc(file)) != EOF)
        {
            putchar(c);
            if(c == '\n')
            {
                lineNum++;
                printf("%d :",lineNum);
            }
        }
        fclose(file);
    }
}

int main(int argc, char **argv)
{
    Init(argv[1]);
    printf("PROGRAM:\n");
    printTheProgram(argv[1]);
    printf("RUNNING....\n");
    Expression();
    return 0;
}
