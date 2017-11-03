/* HashMap.c: Implements various functions defined in HashMap.h */
/* Also defines and implemtents various static functions to help with speicifc tasks */

#include "HashMap.h"
#include "Memory.h"
#include <stdio.h>
#include <string.h>

/* Doubles the capacity of a map and then moves the ndes from the old array to the new array */
/* Param1 HashMap *: The map to be increeased in capacity */
static void PlusMap(HashMap *);

/* Removes all nodes and memory used in a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: Th encompassing map for the node list */
/* Param2 MapNode *: The node list to be ended */
static void EndNode(HashMap *, MapNode *);

/* Removes a specific node from a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: Th encompassing map for the node list */
/* Param2 MapNode *: The node list to search */
/* Param3 char *: The key used to find a matching node in the list */
static MapNode * RemoveNode(HashMap *, MapNode *, char *);

/* Hashes a string using a prime based function */
/* Param1 char *: The string to be hashed */
static uint64_t hash(char *);

/* Initalizes the HashMap and its meta data, must be called before using the HashMap */
/* Returns: A newly allocated HashMap */
HashMap * BuildMap()
{
    HashMap * map;
    uint64_t i;

    /* Initalize the map and its node array */
    map = NewMem(sizeof(HashMap));
    map->nodes = NewMem(sizeof(MapNode *) * 1024);

    /* Set initial metadata values */
    map->count = 0;
    map->size = 1024;
    map->max = 768;

    /* Initialize the array nodes to NULL */
    for (i = 0; i < map->size; ++i)
        map->nodes[i] = NULL;

    return map;
}

/* Adds a key value pair to the map */
/* Param (map) HashMap *: The map to be added to */
/* Param (key) char *: The string key to be hashed and used to identify the key value pair in the map */
/* Param (value) void *: The value to add to the map */
void PushToMap(HashMap * map, char * key, void * value)
{
    MapNode * node;
    uint64_t code, index;

    /* If the map has reached its maximum size, increase the node array capacity */
    if (map->count == map->max)
        PlusMap(map);

    /* Hash the key */
    code = hash(key);
    index = code & (map->size - 1);

    /* Create a new node using the hash, key, and value */
    node = NewMem(sizeof(MapNode));
    node->next = map->nodes[index];
    node->hash = code;
    node->value = value;
    node->key = NewMem(sizeof(char) * (strlen(key) + 1));
    strcpy(node->key, key);

     /* Add the node to the map and increment the node count */
    map->nodes[index] = node;
    ++(map->count);
}

/* Attempts to find a value in the map given some key */
/* Param (map) HashMap *: The map to be searched */
/* Param (key) char *: The string key to be hashed and used to find the value in the map */
/* Returns: The value if found in the map, NULL otherwise */
void * SearchMap(HashMap * map, char * key)
{
    MapNode * node;
    uint64_t index;

    /* Hash the key */
    index = hash(key) & (map->size - 1);
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
    uint64_t index;

    /* Hash the key then remove the matching node from the node list at that specific position in the array */
    index = hash(key) & (map->size - 1);
    map->nodes[index] = RemoveNode(map, map->nodes[index], key);
}

/* Removes all key value pairs from the map */
/* Param (map) HashMap *: The map to be cleared */
void ClearMap(HashMap * map)
{
    uint64_t i;

    /* Clear all node lists in the array */
    for (i = 0; i < map->size; i++)
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
    RemoveMem(map->nodes);
    RemoveMem(map);
}

/* Doubles the capacity of a map and then moves the ndes from the old array to the new array */
/* Param1 HashMap *: The map to be increeased in capacity */
static void PlusMap(HashMap * map)
{
    MapNode ** temp, * next, * node;
    uint64_t capacity, mask, index, i;

    /* Allocate the new node array to be twice the size of the current array */
    capacity = map->size << 1;
    mask = capacity - 1;
    temp = NewMem(sizeof(MapNode *) * capacity);

    /* Initalize the new node array */
    for (i = 0; i < capacity; ++i)
        temp[i] = NULL;

    /* Loop through the old array, moving each node in each node list to the new array */
    for (i = 0; i < map->size; ++i)
    {
        node = map->nodes[i];

        /* Move every node in the node list to the new array using its previously computed hash */
        while (node != NULL)
        {
            index = node->hash & mask;
            next = node->next;
            node->next = temp[index];
            temp[index] = node;
            node = next;
        }
    }

    /* Release the memory used by the old array and update the map's metadata */
    RemoveMem(map->nodes);
    map->nodes = temp;
    map->size = capacity;
    map->max = (capacity * 3) >> 2;
}

/* Removes all nodes and memory used in a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: Th encompassing map for the node list */
/* Param2 MapNode *: The node list to be ended */
static void EndNode(HashMap * map, MapNode * node)
{
    MapNode * next, * temp = node;

    /* Remove all the nodes in the node list and the list itself */
    while (temp != NULL)
    {
        next = temp->next;
        RemoveMem(temp->value);
        RemoveMem(temp);
        --(map->count);
        temp = next;
    }
}

/* Removes a specific node from a node list, updating the encompassing map's count as it does so */
/* Param1 HashMap *: Th encompassing map for the node list */
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
            RemoveMem(node->value);
            RemoveMem(node);
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
            RemoveMem(temp->value);
            RemoveMem(temp);
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
uint64_t hash(char * key)
{
    int len = strlen(key), hash = 0, i;

    for (i = 0; i < len; ++i)
        hash *= (7 + (31 * key[i]));

    return hash;
}
