#include "Memory.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static Memory * mem = NULL;

static void HandleMem(int);
static void * PushMem(unsigned int);
static void PlusMem();
static void FailMem();

void BuildMem()
{
    signal(SIGSEGV, HandleMem);
    signal(SIGINT, HandleMem);
}

void * NewMem(unsigned int size)
{
    void * ptr;
    unsigned int i;

    if (mem != NULL)
        ptr = PushMem(size);
    else
    {
        if ((mem = malloc(sizeof(Memory))) == NULL)
            FailMem();

        if ((mem->values = malloc(sizeof(void *) * 1024)) == NULL)
            FailMem();

        mem->count = 1;
        mem->size = 1024;
        mem->max = 682;

        for (i = 0; i < mem->size; ++i)
            mem->values[i] = NULL;

        if ((ptr = malloc(size)) == NULL)
            FailMem();

        mem->values[(uintptr_t)ptr % 1024] = ptr;
    }

    return ptr;
}

void RemoveMem(void * ptr)
{
    unsigned int index, start;

    if (mem == NULL)
        return;

    index = (uintptr_t)ptr % mem->size;
    start = index;

    while (mem->values[index] != ptr)
    {
        if (++index == mem->size)
            index = 0;

        if (index == start)
            return;
    }

    free(mem->values[index]);
    mem->values[index] = NULL;
    --(mem->count);
}

void NilMem(void ** ptr)
{
    if (ptr == NULL)
        return;

    RemoveMem(*ptr);
    *ptr = NULL;
}

void EndMem()
{
    unsigned int i;

    if (mem == NULL)
        return;

    for (i = 0; i < mem->size; ++i)
        free(mem->values[i]);

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
        case SIGINT:
            EndMem();
            break;
        default:
            fprintf(stderr, "ERROR: unexpected signal (%d) encountered\n", sig);
            EndMem();
    }
}

static void * PushMem(unsigned int size)
{
    void * ptr;
    unsigned int index;

    if (mem->count == mem->max)
        PlusMem();

    if ((ptr = malloc(sizeof(size))) == NULL)
        FailMem();

    index = (uintptr_t)ptr % mem->size;

    while (mem->values[index] != NULL)
    {
        if (++index == mem->size)
            index = 0;
    }

    mem->values[index] = ptr;
    ++(mem->count);

    return ptr;
}

static void PlusMem()
{
    void ** temp;
    void ** old;
    unsigned int index = 0, size, capacity, i;

    size = mem->max;

    if ((old = malloc(sizeof(void *) * size)) == NULL)
        FailMem();

    for (i = 0; i < mem->size; ++i)
    {
        if (mem->values[i] != NULL)
            old[index++] = mem->values[i];
    }

    capacity = (mem->size * 3) >> 1;

    if ((temp = realloc(mem->values, sizeof(void *) * capacity)) == NULL)
        FailMem();

    mem->values = temp;
    mem->count = size;
    mem->size = capacity;
    mem->max = (capacity << 1) / 3;

    for (i = 0; i < mem->size; ++i)
        mem->values[i] = NULL;

    for (i = 0; i < size; ++i)
    {
        index = (uintptr_t)(old[i]) % mem->size;

        while (mem->values[index] != NULL)
        {
            if (++index == mem->size)
                index = 0;
        }

        mem->values[index] = old[i];
    }

    free(old);
}

static void FailMem()
{
    fprintf(stderr, "ERROR: No more memory available for allocation\n");
    EndMem();
    exit(-1);
}
