/* SymbolTable.c: Implements various functions defined in SymbolTable.h */

#include "SymbolTable.h"
#include "Memory.h"

/* Initializes a SymbolTable */
/* NOTE: It's recommended to use the BuildSymbolTable macro instead of calling this directly */
/* Returns: A newly allocated SymbolTable */
SymbolTable * _BuildSymbolTable()
{
    /* Allocate memory for the SymbolTable */
    SymbolTable * st = NULL;
    NewMem(st, sizeof(SymbolTable));

    /* Initialize the SymbolTable information */
    BuildMap(st->values, 1024);
    st->next = NULL;

    return st;

    FAIL:
        /* Free any allocated memory and return NULL on error */
        DiscardMem(st);
        return NULL;
}

/* Releases all memory used by a SymbolTable */
/* Param (st) SymbolTable *: The SymbolTable to be released */
void EndSymbolTable(SymbolTable * st)
{
    /* If st is NULL then we stop since there's nothing we can do */
    if (st == NULL)
        return;

    /* Release all memory used by the SymbolTable and then release the SymbolTable itself */
    EndMap(st->values);
    DiscardMem(st);
}
