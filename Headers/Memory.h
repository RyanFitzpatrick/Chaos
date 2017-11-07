/* Memory.h: Defines various types and prototypes used for memory management */
/* The implementations are defined in Memory.c */

#ifndef _CH_MEMORY_
#define _CH_MEMORY_

/* Memory.h requires the size_t type defined in stdlib.h */
#ifndef _STDLIB_
#define _STDLIB_
#include <stdlib.h>
#endif

/* Allocates a new pointer using malloc */
/* NOTE: If malloc fails this macro will jump to the FAIL label, so the calling code must have a FAIL label */
/* Param1 ptr: The pointer to be allocated */
/* Param2 size: The size of the memory to point at, clean up wit DiscardMem */
#define NewMem(ptr, size) \
    if ((ptr = malloc(size)) == NULL) \
        goto FAIL

/* Releases a pointer using free */
/* Param1 ptr: The pointer to be released */
#define DiscardMem(ptr) free(ptr)

#endif
