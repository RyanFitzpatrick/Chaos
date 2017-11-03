/* Memory.c: Implements various memory management functions defined in Memory.h */
/* Also defines and implemtents various static functions to help with speicifc memory management tasks */
/* Finally this file defines and maintains the single Memory Map (mem) */

#include "Memory.h"
#include <signal.h>
#include <stdio.h>

/* The single Memory Map used and maintained by the program */
static Memory * mem = NULL;

/* The function used to handle various signals */
/* Param1 int: The signal to handle */
static void HandleMem(int);

/* Increases the size of the memory map values array and copies all the old pointers to the new map */
static void PlusMem();

/* Tries to find the encompassing memory node for a pointer in the memory map */
/* Param1 void *: The pointer to find */
/* Returns: The encompassing memory node for the pointer in the memory map, or NULL if the pointer wasn't found */
static MemNode * FindMem(void *);

/* Prints an error message, releases all memory, and exits the program upon a memory failure */
/* Param1 char *: The message to print */
/* Param2 int: The error code */
static void FailMem(char *, int);

/* Releases all memory used by a memory node */
/* Param1 MemNode *: The node to be released */
static void EndNode(MemNode *);

/* Releases a specific pointer from a memory node */
/* Param1 MemNode *: The memory node to search in */
/* Param2 void *: The pointer to search for and release if found, the encompassing node is released from the list as well */
static MemNode * RemoveNode(MemNode *, void *);

/* Initializes the memory map and the signal handlers responsible for handling allocated memory in the event of an interrupt */
/* This must be calld before using any memory functions */
void BuildMem()
{
    uint64_t i;

    /* Attempt to allocate the memory map */
    if ((mem = malloc(sizeof(Memory))) == NULL)
        FailMem("ERROR: No more memory available for allocation", -1);

    /* Attempt to allocate the memory map values array */
    if ((mem->values = malloc(sizeof(MemNode *) * 1024)) == NULL)
        FailMem("ERROR: No more memory available for allocation", -1);

    /* Set intial map values */
    mem->count = 0;
    mem->size = 1024;
    mem->max = 682;

    /* Initialize the map */
    for (i = 0; i < mem->size; ++i)
        mem->values[i] = NULL;

    /* Use the HandleMem function to handle segmentation faults and interrupts */
    signal(SIGSEGV, HandleMem);
    signal(SIGINT, HandleMem);
}

/* Allocates a new pointer and adds it to the memory map */
/* Param (size) size_t: The size of the memory to be allocated */
/* Returns: A pointer to the newly allocated memory, free with either RemoveMem, ClearMem, or EndMem */
void * NewMem(size_t size)
{
    MemNode * node;
    void * ptr;
    uint64_t index;

    /* Don't add anything to the map and simply return NULL if size is specified as 0 */
    if (size == 0 || mem == NULL || mem->values == NULL)
        return NULL;

    /* If the memory map is at capacity then grow it before adding any new entries */
    if (mem->count == mem->max)
        PlusMem();

    /* Attempt to allocate the memory node */
    if ((node = malloc(sizeof(MemNode))) == NULL)
        FailMem("ERROR: No more memory available for allocation", -1);

    /* Attempt to allocate the new pointer */
    if ((ptr = malloc(size)) == NULL)
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

/* Resizes a pointer in the memory map */
/* If the pointer is not in the map then it will be added */
/* Param (ptr) void *: The pointer to be resized */
/* Param (size) size_t: The new size for the pointer */
/* Returns: A pointer to the new memory */
void * ResizeMem(void * ptr, size_t size)
{
    MemNode * node;
    void * temp;

    /* If size is specified as 0 then simply remove the pointer from the memory map */
    if (size == 0)
    {
        RemoveMem(ptr);
        return NULL;
    }

    /* Attempt to find the encompassing node for the pointer */
    node = FindMem(ptr);

    /* If the pointer exists in the memory map then resize it, otherwise simply add it */
    if (node != NULL)
    {
        /* Attempt to allocate the new pointer */
        if ((temp = realloc(ptr, size)) == NULL)
            FailMem("ERROR: No more memory available for allocation", -1);

        /* Update the memory map to use the new pointer and then return it */
        node->value = temp;
        return node->value;
    }
    else
        return NewMem(size);
}

/* Releases and removes a pointer from the memory map */
/* Param (ptr) void *: The pointer to be released and removed */
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

/* Releases, removes, and nulls a pointer from the memory map */
/* Param (ptr) void **: A pointer to the address to be released, removed, and nulled */
void NilMem(void ** ptr)
{
    /* We can't dereference a NULL pointer so we just return */
    if (ptr == NULL)
        return;

    /* Remove the pointer referenced by ptr from the memory map and set it to be null */
    RemoveMem(*ptr);
    *ptr = NULL;
}

/* Releases and removes all memory in the memory map */
/* Note the memory map itself and its values array will still be allocated */
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

/* Releases and removes all memory in the memory map and releases the memory map itself and its values array */
void EndMem()
{
    /* Nothing can be removed from a NULL memory map */
    if (mem == NULL)
        return;

    /* Remove all the memory in use by the memory map and then release the map itself */
    ClearMem();
    free(mem->values);
    free(mem);

    mem = NULL;
}

/* The function used to handle various signals */
/* Param (sig) int: The signal to handle */
static void HandleMem(int sig)
{
    /* Print an error message and release all memory in the memory map when a signal is handled */
    switch (sig)
    {
        case SIGSEGV:
            FailMem("ERROR: segmentation fault encountered\n", SIGSEGV);
            break;
        case SIGINT:
            FailMem("ERROR: program was cancelled while running\n", SIGINT);
            break;
        default:
            FailMem("ERROR: unexpected signal encountered\n", sig);
    }
}

/* Increases the size of the memory map values array and copies all the old pointers to the new map */
static void PlusMem()
{
    MemNode ** temp, * node, * next;
    uint64_t capacity, index, i;

    /* Compute the capacity for the new memory map */
    capacity = (mem->size * 3) >> 1;

    /* Attempt to allocate the new memory map value array */
    if ((temp = malloc(sizeof(MemNode *) * capacity)) == NULL)
        FailMem("ERROR: No more memory available for allocation", -1);

    /* Initialize the memory map value array */
    for (i = 0; i < capacity; ++i)
        temp[i] = NULL;

    for (i = 0; i < mem->size; ++i)
    {
        node = mem->values[i];

        while (node != NULL)
        {
            next = node->next;
            index = (uintptr_t)(node->value) % capacity;
            node->next = temp[index];
            temp[index] = node;
            node = next;
        }
    }

    /* Update the memory map to contain the new values */
    free(mem->values);
    mem->values = temp;
    mem->size = capacity;
    mem->max = (capacity << 1) / 3;
}

/* Tries to find the encompassing memory node for a pointer in the memory map */
/* Param1 void *: The pointer to find */
/* Returns: The encompassing memory node for the pointer in the memory map, or NULL if the pointer wasn't found */
static MemNode * FindMem(void * ptr)
{
    MemNode * node;
    uint64_t index = (uintptr_t)ptr % mem->size;

    node = mem->values[index];

    /* Try to find the pointer in the node list */
    while (node != NULL)
    {
        /* Return the node if the pointer was found */
        if (node->value == ptr)
            return node;

        node = node->next;
    }

    /* Return NULL if the pointer wasn't found */
    return NULL;
}

/* Prints an error message, releases all memory, and exits the program upon a memory failure */
/* Param (message) char *: The message to print */
/* Param (code) int: The error code */
static void FailMem(char * message, int code)
{
    /* Upon some failure, print an error message, release all memory in the memory map, and then exit with a failure code */
    fprintf(stderr, "%s (%d)\n", message, code);
    EndMem();
    exit(code);
}

/* Releases all memory used by a memory node */
/* Param (node) MemNode *: The node to be released */
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

/* Releases a specific pointer from a memory node */
/* Param (node) MemNode *: The memory node to search in */
/* Param (ptr) void *: The pointer to search for and release if found, the encompassing node is released from the list as well */
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
