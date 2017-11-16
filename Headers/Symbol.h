/* Symbol.h: Defines various types and prototypes used for the Symbol type */
/* The implementations are defined in Symbol.c */

#ifndef _CH_SYMBOL_
#define _CH_SYMBOL_

/* This type relies on the Mod and Token enumerations from Enums.h */
/* This type also relies on the Type structure in Type.h */
#include "Enums.h"
#include "Type.h"

/* The structure used to define a Chaos Symbol */
typedef struct Symbol
{
    /* The name of the Symmbol */
    char * name;
    /* The underlying Type of the Symbol */
    Type * type;
    /* The modifier applied to the Symbol (if any) */
    Mod mod;
    /* The Token defining the Symbol */
    Token token;
} Symbol;

/* Initializes a Symbol with the given information */
/* NOTE: It's recommended to use the BuildSymbol macro instead of calling this directly */
/* Param1 char *: The name of the Symbol */
/* Param2 Type *: The underlying Type of the Symbol */
/* Param3 Mod: The modiier applied to the Symbol */
/* Param4 Token: The Token defining the Symbol */
/* Returns: A newly allocated Symbol containing the information passed into the function */
Symbol * _BuildSymbol(char *, Type *, Mod, Token);

/* Releases all memory used by a Symbol */
/* Param1 Symbol *: The Symbol to be released */
void EndSymbol(Symbol *);

/* Calls the _BuildSymbol function and then jumps to the FAIL label on error */
/* This is the recommended way to Build a Symbol */
#define BuildSymbol(symbol, name, type, mod, token) if ((symbol = _BuildSymbol(name, type, mode, token)) == NULL) goto FAIL

#endif
