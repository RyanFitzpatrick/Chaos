#include "HashMap.h"
#include "Memory.h"
#include <stdio.h>
#include <string.h>

static void PlusMap(HashMap *);
static void EndNode(HashMap *, MapNode *);
static MapNode * RemoveNode(HashMap *, MapNode *, char *);
static uint64_t hash(char *);

HashMap * BuildMap()
{
    HashMap * map;
    uint64_t i;

    map = NewMem(sizeof(HashMap));
    map->nodes = NewMem(sizeof(MapNode *) * 1024);

    map->count = 0;
    map->size = 1024;
    map->max = 682;

    for (i = 0; i < map->size; ++i)
        map->nodes[i] = NULL;

    return map;
}

void PushToMap(HashMap * map, char * key, void * value)
{
    MapNode * node;
    uint64_t code, index;

    if (map->count == map->max)
        PlusMap(map);

    code = hash(key);
    index = code % map->size;

    node = NewMem(sizeof(MapNode));
    node->next = map->nodes[index];
    node->hash = code;
    node->value = value;
    node->key = NewMem(sizeof(char) * (strlen(key) + 1));
    strcpy(node->key, key);

    map->nodes[index] = node;
    ++(map->count);
}

void * SearchMap(HashMap * map, char * key)
{
    MapNode * node;
    uint64_t index;

    index = hash(key) % map->size;
    node = map->nodes[index];

    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
            return node->value;

        node = node->next;
    }

    return NULL;
}

void RemoveFromMap(HashMap * map, char * key)
{
    uint64_t index;

    index = hash(key) % map->size;
    map->nodes[index] = RemoveNode(map, map->nodes[index], key);
}

void ClearMap(HashMap * map)
{
    uint64_t i;

    for (i = 0; i < map->size; i++)
    {
        EndNode(map, map->nodes[i]);
        map->nodes[i] = NULL;
    }
}

void EndMap(HashMap * map)
{
    ClearMap(map);
    RemoveMem(map->nodes);
    RemoveMem(map);

    map = NULL;
}

static void PlusMap(HashMap * map)
{
    MapNode ** temp, * next, * node;
    uint64_t capacity, index, i;

    capacity = (map->size * 3) >> 1;
    temp = NewMem(sizeof(MapNode *) * capacity);

    for (i = 0; i < capacity; ++i)
        temp[i] = NULL;

    for (i = 0; i < map->size; ++i)
    {
        node = map->nodes[i];

        while (node != NULL)
        {
            index = node->hash % capacity;
            next = node->next;
            node->next = temp[index];
            temp[index] = node;
            node = next;
        }
    }

    RemoveMem(map->nodes);
    map->nodes = temp;
    map->size = capacity;
    map->max = (capacity << 1) / 3;
}

static void EndNode(HashMap * map, MapNode * node)
{
    MapNode * next, * temp = node;

    while (temp != NULL)
    {
        next = temp->next;
        RemoveMem(temp->value);
        RemoveMem(temp);
        --(map->count);
        temp = next;
    }
}

static MapNode * RemoveNode(HashMap * map, MapNode * node, char * key)
{
    MapNode * prev, * temp;

    if (node == NULL)
        return NULL;

    if (node->next == NULL)
    {
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

    while (temp != NULL)
    {
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

    return node;
}

uint64_t hash(char * key)
{
    int len = strlen(key), hash = 0, i;

    for (i = 0; i < len; ++i)
        hash += (7 + (31 * key[i]));

    return hash;
}
