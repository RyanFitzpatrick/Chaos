/* HashMap.c: Implements various functions defined in HashMap.h */
/* Also defines and implements various static functions to help with specific tasks */

#include "HashMap.h"
#include "Memory.h"
#include <string.h>

/* A static array of prime numbers used as sizes for the HashMap */
static uint32_t primes[NUM_PRIMES] =
{
    2, 5, 11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717,
    51437, 102877, 205759, 411527, 823117, 1646237, 3292489,
    6584983, 13169977, 26339969, 52679987, 105359983,
    210719983, 421439981, 842879963, 1685759983
};

/* Increases the capacity of a map and then moves the nodes from the old array to the new array */
/* Param1 HashMap *: The map who's capacity is to be increased */
static void PlusMap(HashMap *);

/* Removes all nodes and memory used in a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: The encompassing map for the node list */
/* Param2 MapNode *: The node list to be ended */
static void EndNode(HashMap *, MapNode *);

/* Removes a specific node from a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: The encompassing map for the node list */
/* Param2 MapNode *: The node list to search */
/* Param3 char *: The key used to find a matching node in the list */
static MapNode * RemoveNode(HashMap *, MapNode *, char *);

/* Hashes a string using a prime based function */
/* Param1 char *: The string to be hashed */
/* A 32-bit hash value for a string */
static uint32_t hash(char *);

/* Initalizes the HashMap and its meta data, must be called before using the HashMap */
/* NOTE: It's recommended to use the BuildMap macro instead of calling this directly */
/* Param (n) uint32_t: Initial size parameter, the smallest number in primes that is also equal to or larger than n will be the map's size */
/* Returns: A newly allocated HashMap */
HashMap * _BuildMap(uint32_t n)
{
    HashMap * map = NULL;
    uint32_t size = -1, i;

    /* Return NULL if size is specified as 0 */
    if (n == 0)
        return NULL;

    /* Determine the smallest number in primes that is equal to or larger than n, this will be our initial size */
    while (primes[++size] < n);

    /* Initalize the map and its node array */
    NewMem(map, sizeof(HashMap));
    NewMem(map->nodes, sizeof(MapNode *) * primes[size]);

    /* Set initial metadata values */
    map->count = 0;
    map->size = size;
    map->max = (primes[size] * 3) >> 2;

    /* Initialize the array nodes to NULL */
    for (i = 0; i < primes[size]; ++i)
        map->nodes[i] = NULL;

    return map;

    FAIL:
        /* Free any allocated memory and return NULL on error */
        DiscardMem(map);
        return NULL;
}

/* Adds a key value pair to the map */
/* NOTE: It's recommended to use the PushToMap macro instead of calling this directly */
/* Param (map) HashMap *: The map to be added to */
/* Param (key) char *: The string key to be hashed and used to identify the key value pair in the map */
/* Param (value) void *: The value to add to the map */
int _PushToMap(HashMap * map, char * key, void * value)
{
    MapNode * node = NULL;
    uint32_t code, index;

    /* If the map has reached its maximum size, increase the node array capacity if possible */
    if (map->count >= map->max && map->size < NUM_PRIMES)
        PlusMap(map);

    /* Hash the key */
    code = hash(key);
    index = code % primes[map->size];

    /* Create a new node using the hash, key, and value */
    NewMem(node, sizeof(MapNode));
    NewMem(node->key, sizeof(char) * (strlen(key) + 1));
    strcpy(node->key, key);
    node->value = value;
    node->hash = code;
    node->next = map->nodes[index];

     /* Add the node to the map and increment the node count */
    map->nodes[index] = node;
    ++(map->count);
    return 1;

    FAIL:
        /* Free any allocated memory and return -1 on error */
        DiscardMem(node);
        return 0;
}

/* Attempts to find a value in the map given some key */
/* Param (map) HashMap *: The map to be searched */
/* Param (key) char *: The string key to be hashed and used to find the value in the map */
/* Returns: The value if found in the map, NULL otherwise */
void * SearchMap(HashMap * map, char * key)
{
    MapNode * node;
    uint32_t index;

    /* Hash the key */
    index = hash(key) % primes[map->size];
    node = map->nodes[index];

    /* Try and find a matching node in the node list at the specific position in the array */
    while (node != NULL)
    {
        /* If we find a matching key then return the corresponding value */
        if (strcmp(node->key, key) == 0)
            return node->value;

        node = node->next;
    }

    /* If no match was found then return NULL */
    return NULL;
}

/* Removes a key value pair from the map */
/* Param (map) HashMap *: The map to be removed from */
/* Param (key) char *: The string key to be hashed and used to find the value in the map */
void RemoveFromMap(HashMap * map, char * key)
{
    uint32_t index;

    /* Hash the key then remove the matching node from the node list at that specific position in the array */
    index = hash(key) % primes[map->size];
    map->nodes[index] = RemoveNode(map, map->nodes[index], key);
}

/* Removes all key value pairs from the map */
/* Param (map) HashMap *: The map to be cleared */
void ClearMap(HashMap * map)
{
    uint32_t i;

    /* Clear all node lists in the array */
    for (i = 0; i < primes[map->size]; i++)
    {
        EndNode(map, map->nodes[i]);
        map->nodes[i] = NULL;
    }
}

/* Clears and removes the map from memory */
/* Param (map) HashMap *: The map to be ended */
void EndMap(HashMap * map)
{
    /* Clear the array then release the memory used by the array and the map itself */
    ClearMap(map);
    DiscardMem(map->nodes);
    DiscardMem(map);
}

/* Doubles the capacity of a map and then moves the ndes from the old array to the new array */
/* Param1 HashMap *: The map to be increeased in capacity */
static void PlusMap(HashMap * map)
{
    MapNode ** temp = NULL, * next, * node;
    uint32_t size, index, i;

    /* Allocate the new node array using the next number in primes as the size */
    size = primes[map->size + 1];
    NewMem(temp, sizeof(MapNode *) * size);

    /* Initalize the new node array */
    for (i = 0; i < size; ++i)
        temp[i] = NULL;

    /* Loop through the old array, moving each node in each node list to the new array */
    for (i = 0; i < primes[map->size]; ++i)
    {
        node = map->nodes[i];

        /* Move every node in the node list to the new array using its previously computed hash */
        while (node != NULL)
        {
            index = node->hash % size;
            next = node->next;
            node->next = temp[index];
            temp[index] = node;
            node = next;
        }
    }

    /* Release the memory used by the old array and update the map's metadata */
    DiscardMem(map->nodes);
    map->nodes = temp;
    map->max = (primes[++(map->size)] * 3) >> 2;
    return;

    FAIL:
        /* Return on error */
        return;
}

/* Removes all nodes and memory used in a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: The encompassing map for the node list */
/* Param2 MapNode *: The node list to be ended */
static void EndNode(HashMap * map, MapNode * node)
{
    MapNode * next, * temp = node;

    /* Remove all the nodes in the node list and the list itself */
    while (temp != NULL)
    {
        next = temp->next;
        DiscardMem(temp->value);
        DiscardMem(temp->key);
        DiscardMem(temp);
        --(map->count);
        temp = next;
    }
}

/* Removes a specific node from a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: The encompassing map for the node list */
/* Param2 MapNode *: The node list to search */
/* Param3 char *: The key used to find a matching node in the list */
static MapNode * RemoveNode(HashMap * map, MapNode * node, char * key)
{
    MapNode * prev, * temp;

    /* Nothing can be done to a NULL node list */
    if (node == NULL)
        return NULL;

    /* Handle the case where there's only one node in the list */
    if (node->next == NULL)
    {
        /* If the node is a match then remove it, decrement the map's node count, and return NULL, otherwise return the unmodified node */
        if (strcmp(node->value, key) == 0)
        {
            DiscardMem(node->value);
            DiscardMem(node->key);
            DiscardMem(node);
            --(map->count);
            return NULL;
        }
        else
            return node;
    }

    prev = node->next;
    temp = prev->next;

    /* Try to find the matching node in the node list */
    while (temp != NULL)
    {
        /* If the node is a match then remove it, decrement the map's node count, and return the updated node list */
        if (strcmp(temp->value, key) == 0)
        {
            prev->next = temp->next;
            DiscardMem(temp->value);
            DiscardMem(node->key);
            DiscardMem(temp);
            --(map->count);
            return node;
        }

        prev = temp;
        temp = temp->next;
    }

    /* If no match was found then return the unmodified node */
    return node;
}

/* Hashes a string using a prime based function */
/* Param1 char *: The string to be hashed */
/* A 32-bit hash value for a string */
uint32_t hash(char * key)
{
    uint32_t hash = 2166136261;

    /* Compute the hash value using the following funtion */
    while (*key != '\0')
    {
        hash ^= *key++;
        hash *= 16777619;
    }

    return hash;
}
