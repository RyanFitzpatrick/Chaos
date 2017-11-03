#ifndef _CH_SYMBOL_
#define _CH_SYMBOL_

#include "Enums.h"
#include "Type.h"

typedef struct Symbol
{
    char * name;
    Type * type;
    Mod mod;
    Token token;
} Symbol;

Symbol * BuildSymbol(char *, Type *, Mod, Token);
void EndSymbol(Symbol *);

#endif
