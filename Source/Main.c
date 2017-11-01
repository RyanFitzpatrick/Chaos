#include "Memory.h"
#include <stdio.h>

#define SIZE 10000

int main()
{
    int * values[SIZE];
    int i;

    for (i = 0; i < SIZE; ++i)
    {
        values[i] = NewMem(sizeof(int));
        *(values[i]) = i;
    }

    RemoveMem(values[0]);
    EndMem();

    return 0;
}
