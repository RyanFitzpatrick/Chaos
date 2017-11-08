/* Symbol.c: Implements various functions defined in Symbol.h */

#include "Symbol.h"
#include "Memory.h"
#include <string.h>

/* Initializes a Symbol with the given information */
/* NOTE: It's recommended to use the BuildSymbol macro instead of calling this directly */
/* Param (name) char *: The name of the Symbol */
/* Param (type) Type *: The underlying Type of the Symbol */
/* Param (mod) Mod: The modiier applied to the Symbol */
/* Param (token) Token: The Token defining the Symbol */
/* Returns: A newly allocated Symbol containing the information passed into the function */
Symbol * _BuildSymbol(char * name, Type * type, Mod mod, Token token)
{
    /* Allocate memory for the Symbol */
    Symbol * symbol = NULL;
    NewMem(symbol, sizeof(Symbol));

    /* Initialize the Symbol information */
    symbol->token = token;
    symbol->mod = mod;
    symbol->type = type;
    NewMem(symbol->name, sizeof(char) * (strlen(name) + 1));
    strcpy(symbol->name, name);

    return symbol;

    FAIL:
        /* Free any allocated memory and return NULL on error */
        if (symbol != NULL) DiscardMem(symbol->name);
        DiscardMem(symbol);
        return NULL;
}

/* Releases all memory used by a Symbol */
/* Param (symbol) Symbol *: The Symbol to be released */
void EndSymbol(Symbol * symbol)
{
    /* If symbol is NULL then we stop since there's nothing we can do */
    if (symbol == NULL)
        return;

    /* Release all memory used by the Symbol and then release the Symbol itself */
    EndType(symbol->type);
    DiscardMem(symbol->name);
    DiscardMem(symbol);
}
