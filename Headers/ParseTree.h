/* ParseTree.h: Defines various types and prototypes used for the ParseTree type */
/* The implementations are defined in ParseTree.c */

#ifndef _CH_PARSETREE_
#define _CH_PARSETREE_

/* This type relies on the Symbol type in Symbol.h */
#include "Symbol.h"

/* The structure used to define a Chaos ParseTree */
typedef struct ParseTree
{
    /* The Symbol representing the current node of the ParseTree */
    Symbol * symbol;
    /* The child nodes for current ParseTree node */
    struct ParseTree * left;
    struct ParseTree * center;
    struct ParseTree * right;
} ParseTree;

/* Initializes a ParseTree node with the given Symbol */
/* Param1 Symbol *: The Symbol representing the ParseTree node */
/* Returns: A newly allocated ParseTree node */
ParseTree * BuildParseTree(Symbol *);

/* Releases all memory used by a ParseTree */
/* Param1 ParseTree *: The ParseTree to be released */
void EndParseTree(ParseTree *);

#endif
