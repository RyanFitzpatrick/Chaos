/* HashMap.h: Defines various types and prototypes used for the HashMap type */
/* The implementations are defined in HashMap.c */

#ifndef _CH_HASHMAP_
#define _CH_HASHMAP_

/* HashMap.h requires the uint32_t type defined in stdint.h */
#ifndef _STDINT_
#define _STDINT_
#include <stdint.h>
#endif

#define NUM_PRIMES 30

/* The linked list structure used to implement seperate chaining in the memory map */
typedef struct MapNode
{
    /* The node's hash value */
    uint32_t hash;
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
    uint32_t count;
    /* The total length of the memory map's values array */
    uint32_t size;
    /* The maximum number of nodes allowed in the map before the array needs to be expanded */
    uint32_t max;
} HashMap;

/* Initalizes the HashMap and its meta data, must be called before using the HashMap */
/* NOTE: It's recommended to use the BuildMap macro instead of calling this directly */
/* Param1 uint32_t: Initial size parameter, a prime number that is equal to or larger than n will be the map's size */
/* Returns: A newly allocated HashMap */
HashMap * _BuildMap(uint32_t);

/* Adds a key value pair to the map */
/* NOTE: It's recommended to use the PushToMap macro instead of calling this directly */
/* Param1 HashMap *: The map to be added to */
/* Param2 char *: The string key to be hashed and used to identify the key value pair in the map */
/* Param3 void *: The value to add to the map */
int _PushToMap(HashMap *, char *, void *);

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

/* Calls the _BuildMap function and then jumps to the FAIL label on error */
/* This is the recommend way to Build a HashMap */
#define BuildMap(map, size) if ((map = _BuildMap(size)) == NULL) goto FAIL

/* Calls the _PushToMap function and then jumps to the FAIL label on error */
/* This is the recommend way to push a value to a HashMap */
#define PushToMap(map, key, value) if (!_PushToMap(map, key, value)) goto FAIL

#endif
