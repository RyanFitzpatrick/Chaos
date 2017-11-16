#include "Parser.h"
#include "Memory.h"
#include <ctype.h>
#include <string.h>

static ParseTree * ParseToken(char *);
static ParseTree * ParseChar(char);
static int ReadString(ParseStack *, FILE *, char *);

int Parse(ParseStack * stack, SymbolTable * st, FILE * file)
{
    char buffer[32], c = fgetc(file);
    unsigned int count = 0;

    while (!feof(file))
    {
        if (isalnum(c) || c == '_')
        {
            if (count < 31)
                buffer[count++] = c;
            else
            {
                buffer[31] = '\0';
                if (!ReadString(stack, file, buffer)) goto FAIL;
                count = 0;
            }
        }
        else
        {
            buffer[count] = '\0';
            PushToStack(stack, ParseToken(buffer));
            PushToStack(stack, ParseChar(c));
            count = 0;
        }

        c = fgetc(file);
    }

    return 1;

    FAIL:
        return 0;
}

static ParseTree * ParseToken(char * str)
{
    return NULL;
}

static ParseTree * ParseChar(char c)
{
    return NULL;
}

static int ReadString(ParseStack * stack, FILE * file, char * str)
{
    char * buffer = NULL, * temp, c;
    unsigned int count = 32, size = 64;

    NewMem(buffer, sizeof(char) * 64);
    strcpy(buffer, str);
    c = fgetc(file);

    while (isalnum(c) || c == '_')
    {
        if (count == size)
        {
            size <<= 1;
            ResizeMem(temp, buffer, sizeof(char) * size);
        }

        buffer[count++] = c;
        c = fgetc(file);
    }

    buffer[count] = '\0';
    PushToStack(stack, ParseToken(buffer));
    PushToStack(stack, ParseChar(c));

    return 1;

    FAIL:
        DiscardMem(buffer);
        return 0;
}
