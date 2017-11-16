#ifndef _CH_PARSER_
#define _CH_PARSER_

#include "ParseStack.h"
#include "SymbolTable.h"

#ifndef _STDIO
#define _STDIO
#include "stdio.h"
#endif

int Parse(ParseStack *, SymbolTable *, FILE *);

#endif
