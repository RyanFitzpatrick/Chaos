/* SymbolTable.h: Defines various types and prototypes used for the SymbolTable type */
/* The implementations are defined in SymbolTable.c */

#ifndef _CH_SYMBOLTABLE_
#define _CH_SYMBOLTABLE_

/* The SymbolTable relies on the HashMap type from HashMap.h */
#include "HashMap.h"

/* The structure used to define a SymbolTable */
typedef struct SymbolTable
{
    /* The map containing all the Symbols in the Table (HashMap of Symbols) */
    HashMap * values;
    /* A pointer to another SymbolTable, this is used to for handling scope */
    struct SymbolTable * next;
} SymbolTable;

/* Initializes a SymbolTable */
/* NOTE: It's recommended to use the BuildSymbolTable macro instead of calling this directly */
/* Returns: A newly allocated SymbolTable */
SymbolTable * _BuildSymbolTable();

/* Releases all memory used by a SymbolTable */
/* Param1 SymbolTable *: The SymbolTable to be released */
void EndSymbolTable(SymbolTable *);

/* Calls the _BuildSymbolTable function and then jumps to the FAIL label on error */
/* This is the recommended way to Build a SymbolTable */
#define BuildSymbolTable(st) if ((st = _BuildSymbolTable()) == NULL) goto FAIL

#endif
