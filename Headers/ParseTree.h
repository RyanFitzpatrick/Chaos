#ifndef _CH_PARSETREE_
#define _CH_PARSETREE_

#include "Symbol.h"

typedef struct ParseTree {
    Symbol * symbol;
    struct ParseTree * left;
    struct ParseTree * center;
    struct ParseTree * right;
} ParseTree;

ParseTree * BuildParseTree(Symbol *);
void EndParseTree(ParseTree *);

#endif
