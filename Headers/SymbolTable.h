#ifndef _CH_SYMBOLTABLE_
#define _CH_SYMBOLTABLE_

#include "HashMap.h"

typedef struct SymbolTable
{
    HashMap * values;
    struct SymbolTable * next;
} SymbolTable;

SymbolTable * BuildSymbolTable();
void EndSymbolTable(SymbolTable *);

#endif
