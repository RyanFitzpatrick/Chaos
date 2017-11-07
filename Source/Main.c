#include <HashMap.h>
#include "Memory.h"
#include <stdio.h>

#define SIZE 10000

int main()
{
    HashMap * map = NULL;
    char key[10];
    int * ptr = NULL, i;

    map = BuildMap(1024);

    for (i = 0; i < SIZE; ++i)
    {
       sprintf(key, "%d", i);
       NewMem(ptr, sizeof(int));
       *ptr = i;
       PushToMap(map, key, ptr);
    }

    ptr = SearchMap(map, "3000");
    i = ptr != NULL ? *ptr : -1;
    printf("%d\n", i);

    DiscardMem(ptr);
    EndMap(map);

    return 0;

    FAIL:
        free(map);
        free(ptr);

        return 0;
}
