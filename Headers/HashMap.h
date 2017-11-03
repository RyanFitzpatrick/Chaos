/* HashMap.h: Defines various types and prototypes used for the HashMap type */
/* The implementations are defined in HashMap.c */

#ifndef _CH_HASHMAP_
#define _CH_HASHMAP_

/* HashMap.h requires the uint64_t type defined in stdint.h */
#ifndef _STDINT_
#define _STDINT_
#include <stdint.h>
#endif

/* The linked list structure used to implement seperate chaining in the memory map */
typedef struct MapNode
{
    /* The node's hash value */
    uint64_t hash;
    /* The node's key (all keys are strings) */
    char * key;
    /* The node's value */
    void * value;
    /* A pointer linking the current node to the next node */
    struct MapNode * next;
} MapNode;

/* The map structure that stores all the linked key value pairs and meta data */
typedef struct HashMap
{
    /* The array of nodes that store all key value pairs, collisions are handled with seperate chaining */
    MapNode ** nodes;
    /* The current number of nodes in the map */
    uint64_t count;
    /* The total length of the memory map's values array */
    uint64_t size;
    /* The maximum number of nodes allowed in the map before the array needs to be expanded */
    uint64_t max;
} HashMap;

/* Initalizes the HashMap and its meta data, must be called before using the HashMap */
/* Returns: A newly allocated HashMap */
HashMap * BuildMap();

/* Adds a key value pair to the map */
/* Param1 HashMap *: The map to be added to */
/* Param2 char *: The string key to be hashed and used to identify the key value pair in the map */
/* Param3 void *: The value to add to the map */
void PushToMap(HashMap *, char *, void *);

/* Attempts to find a value in the map given some key */
/* Param1 HashMap *: The map to be searched */
/* Param2 char *: The string key to be hashed and used to find the value in the map */
/* Returns: The value if found in the map, NULL otherwise */
void * SearchMap(HashMap *, char *);

/* Removes a key value pair from the map */
/* Param1 HashMap *: The map to be removed from */
/* Param2 char *: The string key to be hashed and used to find the value in the map */
void RemoveFromMap(HashMap *, char *);

/* Removes all key value pairs from the map */
/* Param1 HashMap *: The map to be cleared */
void ClearMap(HashMap *);

/* Clears and removes the map from memory */
/* Param1 HashMap *: The map to be ended */
void EndMap(HashMap *);

#endif
