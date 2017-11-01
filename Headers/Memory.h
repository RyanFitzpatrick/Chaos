#ifndef _CH_MEMORY_
#define _CH_MEMORY_

typedef struct Memory
{
    void ** values;
    unsigned int count;
    unsigned int size;
    unsigned int max;
} Memory;

void * NewMem(unsigned int);
void RemoveMem(void *);
void EndMem();

#endif
