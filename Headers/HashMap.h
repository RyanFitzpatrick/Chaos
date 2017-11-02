#ifndef _CH_HASHMAP_
#define _CH_HASHMAP_

/* HashMap.h requires the uint64_t type defined in stdint.h */
#ifndef _STDINT_
#define _STDINT_
#include <stdint.h>
#endif

typedef struct MapNode
{
    char * key;
    void * value;
    struct MapNode * next;
} MapNode;

typedef struct HashMap
{
    MapNode ** nodes;
    uint64_t count;
    uint64_t size;
    uint64_t max;
} HashMap;

HashMap * BuildMap();
void PushToMap(HashMap *, char *, void *);
void * SearchMap(HashMap *, char *);
void RemoveFromMap(HashMap *, char *);
void ClearMap(HashMap *);
void EndMap(HashMap *);

#endif
