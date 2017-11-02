#include "Memory.h"
#include <signal.h>
#include <stdio.h>

static Memory * mem = NULL;

static void HandleMem(int);
static void * PushMem(size_t);
static void PlusMem();
static void ** DupMem();
static void FailMem(char *, int);
static void EndNode(MemNode *);
static MemNode * RemoveNode(MemNode *, void *);

void BuildMem()
{
    /* Use the HandleMem function to handle segmentation faults and interrupts */
    signal(SIGSEGV, HandleMem);
    signal(SIGINT, HandleMem);
}

void * NewMem(size_t size)
{
    MemNode * node;
    void * ptr;
    uint64_t i;

    /* If memory has already been allocated then simply add the new pointer to the map, otherwise do the initial setup */
    if (mem != NULL)
        ptr = PushMem(size);
    else
    {
        /* Attempt to allocate the memory map */
        if ((mem = malloc(sizeof(Memory))) == NULL)
            FailMem("ERROR: No more memory available for allocation", -1);

        /* Attempt to allocate the memory map values array */
        if ((mem->values = malloc(sizeof(MemNode *) * 1024)) == NULL)
            FailMem("ERROR: No more memory available for allocation", -1);

        /* Set intial map values */
        mem->count = 1;
        mem->size = 1024;
        mem->max = 682;

         /* Initialize the map */
        for (i = 0; i < mem->size; ++i)
            mem->values[i] = NULL;

        /* Attempt to allocate the memory node */
        if ((node = malloc(sizeof(MemNode))) == NULL)
            FailMem("ERROR: No more memory available for allocation", -1);

        /* Attempt to allocate the new pointer */
        if ((ptr = malloc(size)) == NULL)
        {
            free(node);
            FailMem("ERROR: No more memory available for allocation", -1);
        }

        /* Add the new pointer to the map */
        node->value = ptr;
        node->next = NULL;
        mem->values[(intptr_t)ptr % 1024] = node;
    }

    /* Return the newly allocated pointer */
    return ptr;
}

void RemoveMem(void * ptr)
{
    uint64_t index;

    /* Nothing can be removed from a NULL memory map */
    if (mem == NULL)
        return;

    /*Determine the pointers specific position in the map and remove it if it exists */
    index = (uintptr_t)ptr % mem->size;
    mem->values[index] = RemoveNode(mem->values[index], ptr);
}

void NilMem(void ** ptr)
{
    /* We can't dereference a NULL pointer so we just return */
    if (ptr == NULL)
        return;

    /* Remove the pointer referenced by ptr from the memory map and set ptr to be null */
    RemoveMem(*ptr);
    *ptr = NULL;
}

void ClearMem()
{
    uint64_t i;

    /* Nothing can be removed from a NULL memory map */
    if (mem == NULL)
        return;

    /* Remove all the memory in use by the memory map */
    for (i = 0; i < mem->size; i++)
    {
        EndNode(mem->values[i]);
        mem->values[i] = NULL;
    }
}

void EndMem()
{
    /* Nothing can be removed from a NULL memory map */
    if (mem == NULL)
        return;

    /* Remove all the memory in use by the memory map and the release the map itself */
    ClearMem();
    free(mem->values);
    free(mem);

    mem = NULL;
}

static void HandleMem(int sig)
{
    /* Print an error message and release all memory in the memory map when a signal is handled */
    switch (sig)
    {
        case SIGSEGV:
            EndMem("ERROR: segmentation fault encountered\n", SIGSEGV);
            break;
        case SIGINT:
            EndMem("ERROR: program was cancelled while running\n", SIGINT);
            break;
        default:
            EndMem("ERROR: unexpected signal encountered\n", sig);
    }
}

static void * PushMem(size_t size)
{
    MemNode * node;
    void * ptr;
    uint64_t index;

    /* If the memory map is at capacity then grow it before adding any new entries */
    if (mem->count == mem->max)
        PlusMem();

    /* Attempt to allocate the memory node */
    if ((node = malloc(sizeof(MemNode))) == NULL)
        FailMem("ERROR: No more memory available for allocation", -1);

    /* Attempt to allocate the new pointer */
    if ((ptr = malloc(sizeof(size))) == NULL)
    {
        free(node);
        FailMem("ERROR: No more memory available for allocation", -1);
    }

    /* Determine the correct position in the memory map for the pointer and add it to the memory node at that position */
    index = (uintptr_t)ptr % mem->size;
    node->value = ptr;
    node->next = mem->values[index];
    mem->values[index] = node;
    ++(mem->count);

    /* Return the newly allocated pointer */
    return ptr;
}

static void PlusMem()
{
    MemNode ** temp, * node;
    void ** old;
    uint64_t index = 0, capacity, i;

    /* Get a copy of all the currently allocated pointers and compute the capacity for the new memory map */
    old = DupMem();
    capacity = (mem->size * 3) >> 1;

    /* Attempt to allocate the new memory map value array */
    if ((temp = malloc(sizeof(MemNode *) * capacity)) == NULL)
    {
        free(old);
        FailMem("ERROR: No more memory available for allocation", -1);
    }

    /* Update the memory map to contain the new values */
    mem->values = temp;
    mem->size = capacity;
    mem->max = (capacity << 1) / 3;

    /* Initialize the memory map vlue array */
    for (i = 0; i < mem->size; ++i)
        mem->values[i] = NULL;

    /* Add all the previously copied pointers to the new memory map */
    for (i = 0; i < mem->count; ++i)
    {
        /* Attempt to allocate the memory node */
        if ((node = malloc(sizeof(MemNode))) == NULL)
        {
            free(old);
            FailMem("ERROR: No more memory available for allocation", -1);
        }

        /* Determine the correct position for the pointer and add it to the memory map */
        index = (uintptr_t)(old[i]) % mem->size;
        node->value = old[i];
        node->next = mem->values[index];
        mem->values[index] = node;
    }

    /* Release the memory used by the memory copy */
    free(old);
}

static void ** DupMem()
{
    MemNode * next, * node;
    void ** old;
    uint64_t index = 0, i;

    /* Attempt to allocate an array to hold all the pointers */
    if ((old = malloc(sizeof(void *) * mem->count)) == NULL)
        FailMem("ERROR: No more memory available for allocation", -1);

    /* Add all the currently used pointers to the array */
    for (i = 0; i < mem->size; ++i)
    {
        node = mem->values[i];

        /* Add all pointers in the node to the array, release the memory used by the nodes as well */
        while (node != NULL)
        {
            next = node->next;
            old[index++] = node->value;
            free(node);
            node = next;
        }
    }

     /* Release the memory used by the old memory map array and return the copy */
    free(mem->values);
    return old;
}

static void FailMem(char * message, int code)
{
    /* Upon some failure, print an error message, release all memory in the memory map, and then exit with a failure code */
    fprintf(stderr, "%s (%d)\n", message, code);
    EndMem();
    exit(code);
}

static void EndNode(MemNode * node)
{
    MemNode * next, * temp = node;

    /* Release all memory used by a memory node */
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

    /* Nothing can be removed from a NULL memory node */
    if (node == NULL)
        return NULL;

    /* Handle the case where there's only one pointer in the node */
    if (node->next == NULL)
    {
        /* If the pointer matches the one in the node then release it and return NULL, otherwise simply return the unmodified node */
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

    /* Try and find the pointer in the node */
    while (temp != NULL)
    {
        /* If we find a matching pointer then release it and return the updated node */
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

     /* If the pointer wasn't found simply return the unmodified node */
    return node;
}
