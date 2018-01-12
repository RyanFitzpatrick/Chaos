#include <HashMap.h>
#include "Memory.h"
#include <stdio.h>

#define SIZE 10000

int main()
{
    HashMap * map = NULL;
    int * ptr = NULL, i;

   BuildMap(map, 1024);

    for (i = 0; i < SIZE; ++i)
    {
       NewMem(ptr, sizeof(int));
       *ptr = i;
       PushToMap(map, "a", ptr);
    }

    printf("Done\n");
	getchar();
    EndMap(map);

    return 0;

    FAIL:
        printf("Fail\n");
        DiscardMem(map);
        DiscardMem(ptr);

        return 0;
}
