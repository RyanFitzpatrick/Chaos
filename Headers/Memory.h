/* Memory.h: Defines various types and prototypes used for memory management */
/* The implementations are defined in Memory.c */

#ifndef _CH_MEMORY_
#define _CH_MEMORY_

/* Memory.h requires the uint64_t type defined in stdint.h */
#ifndef _STDINT_
#define _STDINT_
#include <stdint.h>
#endif

/* Memory.h requires the size_t type defined in stdlib.h */
#ifndef _STDLIB_
#define _STDLIB_
#include <stdlib.h>
#endif

/* The linked list structure used to implement seperate chaining in the memory map */
typedef struct MemNode
{
    /* The allocated pointer being stored by the node */
    void * value;
    /* A pointer linking the current node to the next node */
    struct MemNode * next;
} MemNode;

/* The Memory HashMap use to manage and store all allocated resources in one place */
typedef struct Memory
{
    /* The array of nodes that store all allocated pointers, collisions are handled with seperate chaining */
    MemNode ** values;
    /* The current number of pointers in the map */
    uint64_t count;
    /* The total length of the memory map's values array */
    uint64_t size;
    /* The maximum number of pointers allowed in the map before the array needs to be expanded */
    uint64_t max;
} Memory;

/* Initializes the memory map and the signal handlers responsible for handling allocated memory in the event of an interrupt */
/* This must be called before using any memory functions */
void BuildMem();

/* Allocates a new pointer and adds it to the memory map */
/* Param1 size_t: The size of the memory to be allocated */
/* Returns: A pointer to the newly allocated memory, free with either RemoveMem, ClearMem, or EndMem */
void * NewMem(size_t);

/* Resizes a pointer in the memory map */
/* If the pointer is not in the map then it will be added */
/* Param1 void *: The pointer to be resized */
/* Param2 size_t: The new size for the pointer */
/* Returns: A pointer to the new memory */
void * ResizeMem(void *, size_t);

/* Releases and removes a pointer from the memory map */
/* Param1 void *: The pointer to be released and removed */
void RemoveMem(void *);

/* Releases, removes, and nulls a pointer from the memory map */
/* Param1 void **: A pointer to the address to be released, removed, and nulled */
void NilMem(void **);

/* Releases and removes all memory in the memory map */
/* Note the memory map itself and its values array will still be allocated */
void ClearMem();

/* Releases and removes all memory in the memory map and releases the memory map itself and its values array */
void EndMem();

#endif
