#include <HashMap.h>
#include "Memory.h"
#include <stdio.h>

#define SIZE 10000

int main()
{
    HashMap * map;
    char key[10];
    int * ptr, i;

    BuildMem();
    map = BuildMap();

    for (i = 0; i < SIZE; ++i)
    {
       sprintf(key, "%d", i);
       ptr = NewMem(sizeof(int));
       *ptr = i;
       PushToMap(map, key, ptr);
    }

    ptr = SearchMap(map, "3000");
    i = ptr != NULL ? *ptr : -1;
    printf("%d\n", i);
    EndMem();

    return 0;
}
