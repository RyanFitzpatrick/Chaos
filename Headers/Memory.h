#ifndef _CH_MEMORY_
#define _CH_MEMORY_

#ifndef _STDINT_
#define _STDINT_
#include <stdint.h>
#endif

#ifndef _STDLIB_
#define _STDLIB_
#include <stdlib.h>
#endif

typedef struct MemNode
{
    void * value;
    struct MemNode * next;
} MemNode;

typedef struct Memory
{
    MemNode ** values;
    uint64_t count;
    uint64_t size;
    uint64_t max;
} Memory;

void BuildMem();
void * NewMem(size_t);
void RemoveMem(void *);
void NilMem(void **);
void ClearMem();
void EndMem();

#endif
