#include "Memory.h"

int main()
{
    int i;

    for (i = 0; i < 10; ++i)
        NewMem(sizeof(int));

    EndMem();
    return 0;
}
