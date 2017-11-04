/* SymbolTable.c: Implements various functions defined in SymbolTable.h */

#include "SymbolTable.h"
#include "Memory.h"

/* Initializes a SymbolTable */
/* Returns: A newly allocated SymbolTable */
SymbolTable * BuildSymbolTable() {
    /* Allocate memory for the SymbolTable */
    SymbolTable * st = NewMem(sizeof(SymbolTable));

    /* Initialize the SymbolTable information */
    st->values = BuildMap(1024);
    st->next = NULL;

    return st;
}

/* Releases all memory used by a SymbolTable */
/* Param (st) SymbolTable *: The SymbolTable to be released */
void EndSymbolTable(SymbolTable * st) {
    /* If st is NULL then we stop since there's nothing we can do */
    if (st == NULL)
        return;

    /* Release all memory used by the SymbolTable and then release the SymbolTable itself */
    EndMap(st->values);
    RemoveMem(st);
}
