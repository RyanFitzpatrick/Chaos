#include "HashMap.h"
#include "Memory.h"
#include <stdio.h>
#include <string.h>

static void PushMap(HashMap *, char *, void *);
static void PlusMap(HashMap *);
static MapNode * DupMap(HashMap *);
static void EndNode(HashMap *, MapNode *);
static MapNode * RemoveNode(HashMap *, MapNode *, char *);
static uint64_t hash(char *, int64_t);

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

    if (map != NULL)
        PushMap(map, key, value);
    else
    {
        map = BuildMap();

        node = NewMem(sizeof(MapNode));
        node->next = NULL;
        node->value = value;
        node->key = NewMem(sizeof(char) * (strlen(key) + 1));
        strcpy(node->key, key);

        map->nodes[hash(key, map->size)] = node;
    }
}

void * SearchMap(HashMap * map, char * key)
{
    MapNode * node;
    uint64_t index;

    if (map == NULL)
        return NULL;

    index = hash(key, map->size);
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

    if (map == NULL)
        return;

    index = hash(key, map->size);
    map->nodes[index] = RemoveNode(map, map->nodes[index], key);
}

void ClearMap(HashMap * map)
{
    uint64_t i;

    if (map == NULL)
        return;

    for (i = 0; i < map->size; i++)
    {
        EndNode(map, map->nodes[i]);
        map->nodes[i] = NULL;
    }
}

void EndMap(HashMap * map)
{
    if (map == NULL)
        return;

    ClearMap(map);
    free(map->nodes);
    free(map);

    map = NULL;
}

static void PushMap(HashMap * map, char * key, void * value)
{
    MapNode * node;
    uint64_t index;

    if (map->count == map->max)
        PlusMap(map);

    index = hash(key, map->size);

    node = NewMem(sizeof(MapNode));
    node->next = map->nodes[index];
    node->value = value;
    node->key = NewMem(sizeof(char) * (strlen(key) + 1));
    strcpy(node->key, key);

    map->nodes[index] = node;
    ++(map->count);
}

static void PlusMap(HashMap * map)
{
    MapNode * node, * old;
    uint64_t index = 0, capacity, i;

    old = DupMap(map);
    capacity = (map->size * 3) >> 1;
    map->nodes = ResizeMem(map->nodes, sizeof(MapNode *) * capacity);

    map->size = capacity;
    map->max = (capacity << 1) / 3;

    for (i = 0; i < map->size; ++i)
        map->nodes[i] = NULL;

    for (i = 0; i < map->count; ++i)
    {
        index = hash(old[i].key, map->size);

        node = NewMem(sizeof(MapNode));
        node->key = old[i].key;
        node->value = old[i].value;
        node->next = map->nodes[index];

        map->nodes[index] = node;
    }

    RemoveMem(old);
}

static MapNode * DupMap(HashMap * map)
{
    MapNode * next, * node, * old;
    uint64_t index = 0, i;

    old = NewMem(sizeof(MapNode) * map->count);

    for (i = 0; i < map->size; ++i)
    {
        node = map->nodes[i];

        while (node != NULL)
        {
            next = node->next;
            old[index].next = NULL;
            old[index].key = node->key;
            old[index++].value = node->value;
            RemoveMem(node);
            node = next;
        }
    }

    RemoveMem(map->nodes);
    return old;
}

static void EndNode(HashMap * map, MapNode * node)
{
    MapNode * next, * temp = node;

    while (temp != NULL)
    {
        next = temp->next;
        free(temp->value);
        free(temp);
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
            free(node->value);
            free(node);
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
            free(temp->value);
            free(temp);
            --(map->count);
            return node;
        }

        prev = temp;
        temp = temp->next;
    }

    return node;
}

uint64_t hash(char * key, int64_t size)
{
    int len = strlen(key), hash = 0, i;

    for (i = 0; i < len; ++i)
        hash += 7 + (31 * key[i]);

    return hash % size;
}
