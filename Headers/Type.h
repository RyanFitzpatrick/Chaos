/* Type.h: Defines various types and prototypes used for the Type structure */
/* The implementations are defined in Type.c */

#ifndef _CH_TYPE
#define _CH_TYPE

/* The Type structure relies on the HashMap type from HashMap.h */
#include "HashMap.h"

/* The structure used to define a Chaos Type */
typedef struct Type {
    /* The name of the Type */
    char * name;
    /* The fields that make up the Type (HashMap of Symbols) */
    HashMap * fields;
    /* The Type that this Type directly inherits from (if any) */
    struct Type * parent;
} Type;

/* Initializes a Type structure */
/* Param1 char *: The name of the Type */
/* Param2 Type *: The Type's parent, use NULL if the Type has no parent */
/* Returns: A newly allocated Type structure containing the information passed into the function */
Type * BuildType(char *, Type *);

/* Releases all memory used by a Type structure */
/* Param1 Type *: The Type to be released */
void EndType(Type *);

#endif
