#include "Memory.h"
#include <signal.h>
#include <stdio.h>

static Memory * mem = NULL;

static void HandleMem(int);
static void * PushMem(size_t);
static void PlusMem();
static void ** DupMem();
static void FailMem();
static void EndNode(MemNode *);
static MemNode * RemoveNode(MemNode *, void *);

void BuildMem()
{
    signal(SIGSEGV, HandleMem);
    signal(SIGINT, HandleMem);
}

void * NewMem(size_t size)
{
    MemNode * node;
    void * ptr;
    uint64_t i;

    if (mem != NULL)
        ptr = PushMem(size);
    else
    {
        if ((mem = malloc(sizeof(Memory))) == NULL)
            FailMem();

        if ((mem->values = malloc(sizeof(MemNode *) * 1024)) == NULL)
            FailMem();

        mem->count = 1;
        mem->size = 1024;
        mem->max = 682;

        for (i = 0; i < mem->size; ++i)
            mem->values[i] = NULL;

        if ((ptr = malloc(size)) == NULL)
            FailMem();

        if ((node = malloc(sizeof(MemNode))) == NULL)
            FailMem();

        node->value = ptr;
        node->next = NULL;
        mem->values[(intptr_t)ptr % 1024] = node;
    }

    return ptr;
}

void RemoveMem(void * ptr)
{
    uint64_t index;

    if (mem == NULL)
        return;

    index = (uintptr_t)ptr % mem->size;
    mem->values[index] = RemoveNode(mem->values[index], ptr);
}

void NilMem(void ** ptr)
{
    if (ptr == NULL)
        return;

    RemoveMem(*ptr);
    *ptr = NULL;
}

void ClearMem()
{
    uint64_t i;

    if (mem == NULL)
        return;

    for (i = 0; i < mem->size; i++)
        EndNode(mem->values[i]);
}

void EndMem()
{
    if (mem == NULL)
        return;

    ClearMem();
    free(mem->values);
    free(mem);

    mem = NULL;
}

static void HandleMem(int sig)
{
    switch (sig)
    {
        case SIGSEGV:
            fprintf(stderr, "ERROR: segmentation fault encountered\n");
            EndMem();
            break;
        case SIGINT:
            fprintf(stderr, "ERROR: program was cancelled while running\n");
            EndMem();
            break;
        default:
            fprintf(stderr, "ERROR: unexpected signal (%d) encountered\n", sig);
            EndMem();
    }
}

static void * PushMem(size_t size)
{
    MemNode * node;
    void * ptr;
    uint64_t index;

    if (mem->count == mem->max)
        PlusMem();

    if ((ptr = malloc(sizeof(size))) == NULL)
        FailMem();

    if ((node = malloc(sizeof(MemNode))) == NULL)
        FailMem();

    index = (uintptr_t)ptr % mem->size;
    node->value = ptr;
    node->next = mem->values[index];
    mem->values[index] = node;
    ++(mem->count);

    return ptr;
}

static void PlusMem()
{
    MemNode ** temp, * node;
    void ** old;
    uint64_t index = 0, size, capacity, i;

    old = DupMem();
    size = mem->max;
    capacity = (mem->size * 3) >> 1;

    if ((temp = malloc(sizeof(MemNode *) * capacity)) == NULL)
    {
        free(old);
        FailMem();
    }

    mem->values = temp;
    mem->count = size;
    mem->size = capacity;
    mem->max = (capacity << 1) / 3;

    for (i = 0; i < mem->size; ++i)
        mem->values[i] = NULL;

    for (i = 0; i < size; ++i)
    {
        if ((node = malloc(sizeof(MemNode))) == NULL)
        {
            free(old);
            FailMem();
        }

        index = (uintptr_t)(old[i]) % mem->size;
        node->value = old[i];
        node->next = mem->values[index];
        mem->values[index] = node;
    }

    free(old);
}

static void ** DupMem()
{
    MemNode * next, * node;
    void ** old;
    uint64_t index = 0, i;

    if ((old = malloc(sizeof(void *) * mem->max)) == NULL)
        FailMem();

    for (i = 0; i < mem->size; ++i)
    {
        node = mem->values[i];

        while (node != NULL)
        {
            next = node->next;
            old[index++] = node->value;
            free(node);
            node = next;
        }
    }

    free(mem->values);
    return old;
}

static void FailMem()
{
    fprintf(stderr, "ERROR: No more memory available for allocation\n");
    EndMem();
    exit(-1);
}

static void EndNode(MemNode * node)
{
    MemNode * next, * temp = node;

    while (temp != NULL)
    {
        next = temp->next;
        free(temp->value);
        free(temp);
        --(mem->count);
        temp = next;
    }
}

static MemNode * RemoveNode(MemNode * node, void * ptr)
{
    MemNode * prev, * temp;

    if (node == NULL)
        return NULL;

    if (node->next == NULL)
    {
        if (node->value == ptr)
        {
            free(node->value);
            free(node);
            --(mem->count);
            return NULL;
        }
        else
            return node;
    }

    prev = node->next;
    temp = prev->next;

    while (temp != NULL)
    {
        if (temp->value == ptr)
        {
            prev->next = temp->next;
            free(temp->value);
            free(temp);
            --(mem->count);
            return node;
        }

        prev = temp;
        temp = temp->next;
    }

    return node;
}
