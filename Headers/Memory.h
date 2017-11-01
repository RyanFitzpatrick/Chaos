#ifndef _CH_MEMORY_
#define _CH_MEMORY_

typedef struct Memory
{
    void ** values;
    int count;
    int size;
    int max;
} Memory;

void * NewMem(unsigned int);
void RemoveMem(void *);
void EndMem();

#endif
