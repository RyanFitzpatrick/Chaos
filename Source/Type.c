/* Type.c: Implements various functions defined in Type.h */

#include "Type.h"
#include "Memory.h"
#include <string.h>

/* Initializes a Type structure */
/* NOTE: It's recommended to use the BuildType macro instead of calling this directly */
/* Param (name) char *: The name of the Type */
/* Param (parent) Type *: The Type's parent, use NULL if the Type has no parent */
/* Returns: A newly allocated Type structure containing the information passed into the function */
Type * _BuildType(char * name, Type * parent)
{
    /* Allocate memory for the Type structure */
    Type * type = NULL;
    NewMem(type, sizeof(Type));

    /* Initialize the Type information */
    BuildMap(type->fields, 32);
    NewMem(type->name, sizeof(char) * (strlen(name) + 1));
    strcpy(type->name, name);
    type->parent = parent;

    return type;

    FAIL:
        /* Free any allocated memory and return NULL on error */
        DiscardMem(type);
        return NULL;
}

/* Releases all memory used by a Type structure */
/* Param (type) Type *: The Type to be released */
void EndType(Type * type)
{
    /* If type is NULL then we stop since there's nothing we can do */
    if (type == NULL)
        return;

    /* Release all memory used by the Type structure and then release the Type structure itself */
    EndMap(type->fields);
    DiscardMem(type->name);
    DiscardMem(type);
}
