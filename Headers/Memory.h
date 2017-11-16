/* Memory.h: Defines various macros used for memory management */

#ifndef _CH_MEMORY_
#define _CH_MEMORY_

/* Memory.h requires the malloc and free functions defined in stdlib.h */
#ifndef _STDLIB_
#define _STDLIB_
#include <stdlib.h>
#endif

/* Allocates a new pointer using malloc */
/* NOTE: If malloc fails this macro will jump to the FAIL label, so the calling code must have a FAIL label */
/* Param1 ptr: The pointer to be allocated */
/* Param2 size: The size of the memory to point at, clean up with DiscardMem */
#define NewMem(ptr, size) if ((ptr = malloc(size)) == NULL) goto FAIL

/* Resizes a pointer using realloc */
/* NOTE: If realloc fails this macro will jump to the FAIL label, so the calling code must have a FAIL label */
/* Param1 temp: A temp pointer used to hold the return value of realloc and check for any error */
/* Param2 ptr: The pointer to be reallocated */
/* Param2 size: The size of the memory to point at, clean up with DiscardMem */
#define ResizeMem(temp, ptr, size) \
{ \
    if ((temp = realloc(ptr, size)) == NULL) \
        goto FAIL; \
\
    ptr = temp \
}

/* Releases a pointer using free */
/* Param1 ptr: The pointer to be released */
#define DiscardMem(ptr) free(ptr)

#endif
