#include "SymbolTable.h"
#include "Memory.h"

SymbolTable * BuildSymbolTable()
{
    SymbolTable * st = NewMem(sizeof(SymbolTable));

    st->values = BuildMap(1024);
    st->next = NULL;

    return st;
}

void EndSymbolTable(SymbolTable * st)
{
    if (st == NULL)
        return;

    EndMap(st->values);
    RemoveMem(st);
}
