/* ParseTree.h: Defines various types and prototypes used for the ParseTree type */
/* The implementations are defined in ParseTree.c */

#ifndef _CH_PARSESTACK_
#define _CH_PARSESTACK_

/* This type relies on the ParseTree type in ParseTree.h */
#include "ParseTree.h"

/* The structure used to define a Chaos ParseTree */
typedef struct ParseStack
{
    /* The ParseTree element for a specific stack node */
    ParseTree * node;
    /* A pointer to the next node stack */
    struct ParseStack * next;
} ParseStack;

/* Initializes an empty ParseStack */
/* Returns: A newly allocated ParseStack */
ParseStack * BuildParseStack();

/* Releases all memory used by a ParseStack */
/* Param1 ParseStack *: The ParseStack to be released */
void EndParseStack(ParseStack *);

#endif
