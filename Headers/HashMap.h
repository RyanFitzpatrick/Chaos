#ifndef _CH_HASHMAP_
#define _CH_HASHMAP_

typedef struct HashMap
{
    void ** values;
    char ** keys;
    int count;
    int size;
    int max;
} HashMap;

HashMap * BuildMap();
void PushToMap(HashMap *, char *, void *);
void RemoveFromMap(HashMap *, char *);
void EndMap(HashMap *);

#endif
