#include "Parser.h"
#include "Memory.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int ReadString(ParseStack *, FILE *, char *);

int Parse(ParseStack * stack, FILE * file)
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
                if (!ReadString(file, buffer)) goto FAIL;
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

static int ReadString(ParseStack * stack, FILE * file, char * str)
{
    char * buffer = NewMem(sizeof(char) * 64), * temp, c;
    unsigned int count = 32, size = 64;

    strcpy(buffer, str);
    c = fgetc(file);

    while (isalnum(c) || c == '_')
    {
        if (count == size)
        {
            size <<= 1;
            buffer = ResizeMem(temp, buffer, sizeof(char) * size);
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
