#include "Symbol.h"
#include "Memory.h"
#include <string.h>

Symbol * BuildSymbol(char * name, Type * type, Mod mod, Token token)
{
    /* Allocate memory for the Symbol */
    Symbol * symbol = NewMem(sizeof(Symbol));

    /* Initialize the Symbol information */
    symbol->token = token;
    symbol->mod = mod;
    symbol->type = type;
    symbol->name = NewMem(sizeof(char) * (strlen(name) + 1));
    strcpy(symbol->name, name);

    return symbol;
}

void EndSymbol(Symbol * symbol)
{
    if (symbol == NULL)
        return;

    EndType(symbol->type);
    RemoveMem(symbol->name);
    RemoveMem(symbol);
}
